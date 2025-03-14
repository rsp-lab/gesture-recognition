using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.Threading;
using System.Text.RegularExpressions;
using System.Runtime.InteropServices;

namespace WykrywanieGestow
{
    public partial class Form1 : Form
    {
        /*
         * import funkcji z windowsowych DLLi za pomoca ktorych dostaniemy sie do innych aplikacji 
         */

        //pobiera uchwyt do pulpitu
        [DllImport("user32")]
        public static extern int GetDesktopWindow();

        //pobiera uchwyt do okna na pulpicie
        [DllImport("user32")]
        public static extern int GetWindow(int hwnd, int wCmd);

        //sprawdza czy okno jest widoczne na ekranie
        [DllImport("user32")]
        public static extern int IsWindowVisible(int hwnd);

        //pobiera tutul okna
        [DllImport("User32.Dll")]
        public static extern void GetWindowText(int h, StringBuilder s, int nMaxCount);

        //wyszukuje okno po nazwie
        [DllImport("USER32.DLL", CharSet = CharSet.Unicode)]
        public static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

        //aktywuje wskazane okno - cokolwiek to znaczy, ale jak sie tego nie zrobi to nie dziala
        [DllImport("USER32.DLL")]
        public static extern bool SetForegroundWindow(IntPtr hWnd);

        /*
         * rozne stale do w/w funkcji z windowsowych DLLi
         */
        public const int GW_HWNDFIRST = 0;
        public const int GW_HWNDLAST = 1;
        public const int GW_HWNDNEXT = 2;
        public const int GW_HWNDPREV = 3;
        public const int GW_OWNER = 4;
        public const int GW_CHILD = 5;
        
        public StringBuilder sortOutput;
        private Process sortProcess;
        private int n = 1;
        private GestureProperties properties;

        public Form1()
        {
            InitializeComponent();
        }

        private void buttonRun_Click(object sender, EventArgs e)
        {
            string haarSingleWidth = textHaarSingleWidth.Text;
            string haarHeight = textHaarHeight.Text;
            string haarStepWidth = textHaarStepWidth.Text;
            string haarStepHeight = textHaarStepHeight.Text;
            string similarPosStraight = textSimilarPosStraight.Text;
            string similarPosLeft = textSimilarPosLeft.Text;
            string similarPosRight = textSimilarPosRight.Text;
            string movementToleranceHorizontal = textMoveToleranceHorizontal.Text;
            string movementToleranceVertical = textMoveToleranceVertical.Text;
            string gestureHorizontal = textGestureHorizontal.Text;
            string gestureVertical = textGestureVertical.Text;
           
            properties = new GestureProperties(textFileProp.Text);

            Regex pattern = new Regex("^[0-9]+$");
            if (!pattern.IsMatch(haarSingleWidth) || !pattern.IsMatch(haarHeight) ||
               !pattern.IsMatch(haarStepWidth) || !pattern.IsMatch(haarStepHeight) ||
               !pattern.IsMatch(similarPosStraight) || !pattern.IsMatch(similarPosLeft) ||
               !pattern.IsMatch(similarPosRight) || !pattern.IsMatch(movementToleranceHorizontal) ||
               !pattern.IsMatch(movementToleranceVertical) || !pattern.IsMatch(gestureHorizontal) ||
               !pattern.IsMatch(gestureVertical))
            {
                MessageBox.Show("Nieprawidłowy parametr!", "Błąd!", MessageBoxButtons.OK, MessageBoxIcon.Stop);
            }
            else
            {
                buttonRun.Enabled = false;
                buttonStop.Enabled = true;

                StringBuilder builder = new StringBuilder();
                builder.
                    Append(haarSingleWidth).Append(" ").
                    Append(haarHeight).Append(" ").
                    Append(haarStepWidth).Append(" ").
                    Append(haarStepHeight).Append(" ").
                    Append(similarPosStraight).Append(" ").
                    Append(similarPosLeft).Append(" ").
                    Append(similarPosRight).Append(" ").
                    Append(movementToleranceHorizontal).Append(" ").
                    Append(movementToleranceVertical).Append(" ").
                    Append(gestureHorizontal).Append(" ").
                    Append(gestureVertical);

                sortProcess = new Process();
                sortProcess.StartInfo.FileName = "CVTest.exe";
                sortProcess.StartInfo.Arguments = builder.ToString();

                // Set UseShellExecute to false for redirection.
                sortProcess.StartInfo.CreateNoWindow = true;
                sortProcess.StartInfo.UseShellExecute = false;

                // Redirect the standard output of the sort command.  
                // This stream is read asynchronously using an event handler.
                sortProcess.StartInfo.RedirectStandardOutput = true;
                sortOutput = new StringBuilder("");

                // Set our event handler to asynchronously read the sort output.
                sortProcess.OutputDataReceived += new DataReceivedEventHandler(SortOutputHandler);

                // Redirect standard input as well.  This stream
                // is used synchronously.
                sortProcess.StartInfo.RedirectStandardInput = true;

                // Start the process.
                sortProcess.Start();

                // Start the asynchronous read of the sort output stream.
                sortProcess.BeginOutputReadLine();
                while (!sortProcess.HasExited)
                {
                    // This keeps your form responsive by processing events
                    Application.DoEvents(); 
                }
            }
        }

        private void SortOutputHandler(object sendingProcess, DataReceivedEventArgs outLine)
        {

            if (richTextConsole.InvokeRequired)
            { 
                richTextConsole.BeginInvoke(new DataReceivedEventHandler(SortOutputHandler), new[] { sendingProcess, outLine });
            }
            else
            {
                sortOutput.Clear();
                sortOutput.Append(Environment.NewLine + outLine.Data);
                string line = sortOutput.ToString();

                if (sortOutput.ToString().Contains("Wykonano gest w "))
                {
                    labelGestureText1.Visible = true;
                    labelGestureText2.Visible = true;
                    labelGestureRecognitor.Visible = true;
                    labelGestureDirection.Visible = true;
                    StringBuilder builder = new StringBuilder();

                    getGesture(line);

                    labelGestureDirection.Text = line.Substring(18);
                    n = 1;
                }

                if (sortOutput.ToString().Contains("Parametry"))
                {
                    richTextConsole.Text = "";
                }
                richTextConsole.AppendText(sortOutput.ToString());

                if (n == 40)
                {
                    labelGestureText1.Visible = false;
                    labelGestureText2.Visible = false;
                    labelGestureRecognitor.Visible = false;
                    labelGestureDirection.Visible = false;
                    n = 1;
                }

                n++;
             }
        }

        /** wywolanie skrotu klawiaturowego */
        private void sendKeystroke(String keystroke)
        {
            if (textApp.Text != null && !textApp.Text.Equals(""))
            {
                //Wyszukanie handlera do aplikacji
                IntPtr handle = getApplicationHandle(textApp.Text);
                if (handle != IntPtr.Zero)
                {
                    SetForegroundWindow(handle);
                    SendKeys.SendWait(keystroke);
                }
                else
                {
                    MessageBox.Show("Nie można znaleźć takiej aplikacji!", "Błąd!", MessageBoxButtons.OK, MessageBoxIcon.Stop);
                }
            }
            else
            {
                //MessageBox.Show("Musisz podać nazwę aplikacji!", "Błąd!", MessageBoxButtons.OK, MessageBoxIcon.Stop);
            }
        }

        /** zwraca uchwyt do podanego okna */
        private IntPtr getApplicationHandle(String title)
        {
            GetDesktopWindow();

            //znajdz okno dokladnie z taka nazwa
            IntPtr handle = FindWindow(null, title);

            //jezeli nie znaleziono z taka nazwa, to szukaj takie, ktore ta nazwe zawiera
            if (handle == IntPtr.Zero)
            {
                //uchwyt do pulpitu
                int nDeshWndHandle = GetDesktopWindow();

                //uchwyt do pierwszego okna na pulpicie
                int nChildHandle = GetWindow(nDeshWndHandle, GW_CHILD);

                //jedziemy po wszystkich oknach na pulpicie do poki nie znajdziemy
                while (nChildHandle != 0)
                {

                    //sprawdzamy czy nie znalazl sam siebie
                    if (nChildHandle == this.Handle.ToInt32())
                    {
                        nChildHandle = GetWindow(nChildHandle, GW_HWNDNEXT);
                    }

                    //sprawdza, czy okno jest widoczne
                    if (IsWindowVisible(nChildHandle) != 0)
                    {
                        StringBuilder sbTitle = new StringBuilder(1024);
                        //odczytuje tytul okna
                        GetWindowText(nChildHandle, sbTitle, sbTitle.Capacity);
                        String sWinTitle = sbTitle.ToString();
                        if (sWinTitle.Length > 0)
                        {
                            if (sWinTitle.IndexOf(title) > -1)
                            {
                                return FindWindow(null, sWinTitle); ;
                            }
                        }
                    }

                    //pobranie kolejnego okna
                    nChildHandle = GetWindow(nChildHandle, GW_HWNDNEXT);
                }
            }

            return handle;
        }

        private void buttonStop_Click(object sender, EventArgs e)
        {
            buttonRun.Enabled = true;
            buttonStop.Enabled = false;
            sortProcess.CancelOutputRead();
            sortProcess.Kill();
            richTextConsole.Text = "";
        }

        private void buttonInfo_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Tytuł aplikacji to nazwa, która jest wyświetlana na jego belce (lub zawiera się w belce).\n\nUwaga, rozróżniana jest wielkość liter! \n\nZnaki specjalne wpisujemy w nawiasach krętych: {ENTER} {ESC} {UP} {DOWN} {LEFT} {RIGHT} itd.\n\nWięcej informacji: http://msdn.microsoft.com/en-us/library/system.windows.forms.sendkeys.aspx \n");
        }

        private void getGesture(string line)
        {
            if (richTextConsole.Text.Contains("|") && richTextConsole.Text.Contains("\\") && richTextConsole.Text.Contains("/"))
            {
                labelGestureRecognitor.Text = "trzy palce";
                if (line.Substring(18).Contains("lewo"))
                {
                    sendKeystroke(properties.get("3fingleft"));
                }
                if (line.Substring(18).Contains("prawo"))
                {
                    sendKeystroke(properties.get("3fingright"));
                }
                if (line.Substring(18).Contains("gora"))
                {
                    sendKeystroke(properties.get("3fingup"));
                }
                if (line.Substring(18).Contains("dol"))
                {
                    sendKeystroke(properties.get("3fingdown"));
                }
            }
            else if ((richTextConsole.Text.Contains("|") && richTextConsole.Text.Contains("\\")) || (richTextConsole.Text.Contains("|") && richTextConsole.Text.Contains("/")) || (richTextConsole.Text.Contains("/") && richTextConsole.Text.Contains("\\")))
            {
                labelGestureRecognitor.Text = "dwa palce";
                if (line.Substring(18).Contains("lewo"))
                {
                    sendKeystroke(properties.get("2fingleft"));
                }
                if (line.Substring(18).Contains("prawo"))
                {
                    sendKeystroke(properties.get("2fingright"));
                }
                if (line.Substring(18).Contains("gora"))
                {
                    sendKeystroke(properties.get("2fingup"));
                }
                if (line.Substring(18).Contains("dol"))
                {
                    sendKeystroke(properties.get("2fingdown"));
                }
            }
            else if (richTextConsole.Text.Contains("|"))
            {
                labelGestureRecognitor.Text = "pionowy";
                if (line.Substring(18).Contains("lewo"))
                {
                    sendKeystroke(properties.get("1fingSleft"));
                }
                if (line.Substring(18).Contains("prawo"))
                {
                    sendKeystroke(properties.get("1fingSright"));
                }
                if (line.Substring(18).Contains("gora"))
                {
                    sendKeystroke(properties.get("1fingSup"));
                }
                if (line.Substring(18).Contains("dol"))
                {
                    sendKeystroke(properties.get("1fingSdown"));
                }
            }
            else if (richTextConsole.Text.Contains("\\"))
            {
                labelGestureRecognitor.Text = "lewy skos";
                if (line.Substring(18).Contains("lewo"))
                {
                    sendKeystroke(properties.get("1fingLleft"));
                }
                if (line.Substring(18).Contains("prawo"))
                {
                    sendKeystroke(properties.get("1fingLright"));
                }
                if (line.Substring(18).Contains("gora"))
                {
                    sendKeystroke(properties.get("1fingLup"));
                }
                if (line.Substring(18).Contains("dol"))
                {
                    sendKeystroke(properties.get("1fingLdown"));
                }
            }
            else if (richTextConsole.Text.Contains("/"))
            {
                labelGestureRecognitor.Text = "prawy skos";
                if (line.Substring(18).Contains("lewo"))
                {
                    sendKeystroke(properties.get("1fingRleft"));
                }
                if (line.Substring(18).Contains("prawo"))
                {
                    sendKeystroke(properties.get("1fingRright"));
                }
                if (line.Substring(18).Contains("gora"))
                {
                    sendKeystroke(properties.get("1fingRup"));
                }
                if (line.Substring(18).Contains("dol"))
                {
                    sendKeystroke(properties.get("1fingRdown"));
                }
            }
        }
    }
}
