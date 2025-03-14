namespace WykrywanieGestow
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.buttonRun = new System.Windows.Forms.Button();
            this.textHaarSingleWidth = new System.Windows.Forms.TextBox();
            this.labelHaarSingleWidth = new System.Windows.Forms.Label();
            this.labelHaarHeight = new System.Windows.Forms.Label();
            this.textHaarHeight = new System.Windows.Forms.TextBox();
            this.labelHaarStepWidth = new System.Windows.Forms.Label();
            this.textHaarStepWidth = new System.Windows.Forms.TextBox();
            this.labelHaarStepHeight = new System.Windows.Forms.Label();
            this.textHaarStepHeight = new System.Windows.Forms.TextBox();
            this.labelSimilarPosStraight = new System.Windows.Forms.Label();
            this.textSimilarPosStraight = new System.Windows.Forms.TextBox();
            this.labelSimilarPosLeft = new System.Windows.Forms.Label();
            this.textSimilarPosLeft = new System.Windows.Forms.TextBox();
            this.labelSimilarPosRight = new System.Windows.Forms.Label();
            this.textSimilarPosRight = new System.Windows.Forms.TextBox();
            this.labelGestureVertical = new System.Windows.Forms.Label();
            this.textGestureVertical = new System.Windows.Forms.TextBox();
            this.labelGestureHorizontal = new System.Windows.Forms.Label();
            this.textGestureHorizontal = new System.Windows.Forms.TextBox();
            this.labelMoveToleranceVertical = new System.Windows.Forms.Label();
            this.textMoveToleranceVertical = new System.Windows.Forms.TextBox();
            this.labelMoveToleranceHorizontal = new System.Windows.Forms.Label();
            this.textMoveToleranceHorizontal = new System.Windows.Forms.TextBox();
            this.richTextConsole = new System.Windows.Forms.RichTextBox();
            this.groupConfiguration = new System.Windows.Forms.GroupBox();
            this.groupConsole = new System.Windows.Forms.GroupBox();
            this.buttonStop = new System.Windows.Forms.Button();
            this.labelGestureText1 = new System.Windows.Forms.Label();
            this.labelGestureDirection = new System.Windows.Forms.Label();
            this.labelGestureText2 = new System.Windows.Forms.Label();
            this.labelGestureRecognitor = new System.Windows.Forms.Label();
            this.textApp = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.labelFileProp = new System.Windows.Forms.Label();
            this.textFileProp = new System.Windows.Forms.TextBox();
            this.buttonInfo = new System.Windows.Forms.Button();
            this.groupConfiguration.SuspendLayout();
            this.groupConsole.SuspendLayout();
            this.SuspendLayout();
            // 
            // buttonRun
            // 
            this.buttonRun.Location = new System.Drawing.Point(326, 217);
            this.buttonRun.Name = "buttonRun";
            this.buttonRun.Size = new System.Drawing.Size(119, 41);
            this.buttonRun.TabIndex = 0;
            this.buttonRun.Text = "Run";
            this.buttonRun.UseVisualStyleBackColor = true;
            this.buttonRun.Click += new System.EventHandler(this.buttonRun_Click);
            // 
            // textHaarSingleWidth
            // 
            this.textHaarSingleWidth.Location = new System.Drawing.Point(170, 24);
            this.textHaarSingleWidth.Name = "textHaarSingleWidth";
            this.textHaarSingleWidth.Size = new System.Drawing.Size(100, 20);
            this.textHaarSingleWidth.TabIndex = 1;
            this.textHaarSingleWidth.Text = "10";
            // 
            // labelHaarSingleWidth
            // 
            this.labelHaarSingleWidth.AutoSize = true;
            this.labelHaarSingleWidth.Location = new System.Drawing.Point(75, 27);
            this.labelHaarSingleWidth.Name = "labelHaarSingleWidth";
            this.labelHaarSingleWidth.Size = new System.Drawing.Size(89, 13);
            this.labelHaarSingleWidth.TabIndex = 2;
            this.labelHaarSingleWidth.Text = "haar single width:";
            // 
            // labelHaarHeight
            // 
            this.labelHaarHeight.AutoSize = true;
            this.labelHaarHeight.Location = new System.Drawing.Point(101, 53);
            this.labelHaarHeight.Name = "labelHaarHeight";
            this.labelHaarHeight.Size = new System.Drawing.Size(63, 13);
            this.labelHaarHeight.TabIndex = 4;
            this.labelHaarHeight.Text = "haar height:";
            // 
            // textHaarHeight
            // 
            this.textHaarHeight.Location = new System.Drawing.Point(170, 50);
            this.textHaarHeight.Name = "textHaarHeight";
            this.textHaarHeight.Size = new System.Drawing.Size(100, 20);
            this.textHaarHeight.TabIndex = 3;
            this.textHaarHeight.Text = "15";
            // 
            // labelHaarStepWidth
            // 
            this.labelHaarStepWidth.AutoSize = true;
            this.labelHaarStepWidth.Location = new System.Drawing.Point(82, 79);
            this.labelHaarStepWidth.Name = "labelHaarStepWidth";
            this.labelHaarStepWidth.Size = new System.Drawing.Size(82, 13);
            this.labelHaarStepWidth.TabIndex = 6;
            this.labelHaarStepWidth.Text = "haar step width:";
            // 
            // textHaarStepWidth
            // 
            this.textHaarStepWidth.Location = new System.Drawing.Point(170, 76);
            this.textHaarStepWidth.Name = "textHaarStepWidth";
            this.textHaarStepWidth.Size = new System.Drawing.Size(100, 20);
            this.textHaarStepWidth.TabIndex = 5;
            this.textHaarStepWidth.Text = "5";
            // 
            // labelHaarStepHeight
            // 
            this.labelHaarStepHeight.AutoSize = true;
            this.labelHaarStepHeight.Location = new System.Drawing.Point(78, 105);
            this.labelHaarStepHeight.Name = "labelHaarStepHeight";
            this.labelHaarStepHeight.Size = new System.Drawing.Size(86, 13);
            this.labelHaarStepHeight.TabIndex = 8;
            this.labelHaarStepHeight.Text = "haar step height:";
            // 
            // textHaarStepHeight
            // 
            this.textHaarStepHeight.Location = new System.Drawing.Point(170, 102);
            this.textHaarStepHeight.Name = "textHaarStepHeight";
            this.textHaarStepHeight.Size = new System.Drawing.Size(100, 20);
            this.textHaarStepHeight.TabIndex = 7;
            this.textHaarStepHeight.Text = "10";
            // 
            // labelSimilarPosStraight
            // 
            this.labelSimilarPosStraight.AutoSize = true;
            this.labelSimilarPosStraight.Location = new System.Drawing.Point(69, 131);
            this.labelSimilarPosStraight.Name = "labelSimilarPosStraight";
            this.labelSimilarPosStraight.Size = new System.Drawing.Size(95, 13);
            this.labelSimilarPosStraight.TabIndex = 10;
            this.labelSimilarPosStraight.Text = "similarPos Straight:";
            // 
            // textSimilarPosStraight
            // 
            this.textSimilarPosStraight.Location = new System.Drawing.Point(170, 128);
            this.textSimilarPosStraight.Name = "textSimilarPosStraight";
            this.textSimilarPosStraight.Size = new System.Drawing.Size(100, 20);
            this.textSimilarPosStraight.TabIndex = 9;
            this.textSimilarPosStraight.Text = "5";
            // 
            // labelSimilarPosLeft
            // 
            this.labelSimilarPosLeft.AutoSize = true;
            this.labelSimilarPosLeft.Location = new System.Drawing.Point(87, 157);
            this.labelSimilarPosLeft.Name = "labelSimilarPosLeft";
            this.labelSimilarPosLeft.Size = new System.Drawing.Size(77, 13);
            this.labelSimilarPosLeft.TabIndex = 12;
            this.labelSimilarPosLeft.Text = "similarPos Left:";
            // 
            // textSimilarPosLeft
            // 
            this.textSimilarPosLeft.Location = new System.Drawing.Point(170, 154);
            this.textSimilarPosLeft.Name = "textSimilarPosLeft";
            this.textSimilarPosLeft.Size = new System.Drawing.Size(100, 20);
            this.textSimilarPosLeft.TabIndex = 11;
            this.textSimilarPosLeft.Text = "4";
            // 
            // labelSimilarPosRight
            // 
            this.labelSimilarPosRight.AutoSize = true;
            this.labelSimilarPosRight.Location = new System.Drawing.Point(80, 183);
            this.labelSimilarPosRight.Name = "labelSimilarPosRight";
            this.labelSimilarPosRight.Size = new System.Drawing.Size(84, 13);
            this.labelSimilarPosRight.TabIndex = 14;
            this.labelSimilarPosRight.Text = "similarPos Right:";
            // 
            // textSimilarPosRight
            // 
            this.textSimilarPosRight.Location = new System.Drawing.Point(170, 180);
            this.textSimilarPosRight.Name = "textSimilarPosRight";
            this.textSimilarPosRight.Size = new System.Drawing.Size(100, 20);
            this.textSimilarPosRight.TabIndex = 13;
            this.textSimilarPosRight.Text = "4";
            // 
            // labelGestureVertical
            // 
            this.labelGestureVertical.AutoSize = true;
            this.labelGestureVertical.Location = new System.Drawing.Point(82, 286);
            this.labelGestureVertical.Name = "labelGestureVertical";
            this.labelGestureVertical.Size = new System.Drawing.Size(82, 13);
            this.labelGestureVertical.TabIndex = 22;
            this.labelGestureVertical.Text = "gesture vertical:";
            // 
            // textGestureVertical
            // 
            this.textGestureVertical.Location = new System.Drawing.Point(170, 283);
            this.textGestureVertical.Name = "textGestureVertical";
            this.textGestureVertical.Size = new System.Drawing.Size(100, 20);
            this.textGestureVertical.TabIndex = 21;
            this.textGestureVertical.Text = "5";
            // 
            // labelGestureHorizontal
            // 
            this.labelGestureHorizontal.AutoSize = true;
            this.labelGestureHorizontal.Location = new System.Drawing.Point(71, 260);
            this.labelGestureHorizontal.Name = "labelGestureHorizontal";
            this.labelGestureHorizontal.Size = new System.Drawing.Size(93, 13);
            this.labelGestureHorizontal.TabIndex = 20;
            this.labelGestureHorizontal.Text = "gesture horizontal:";
            // 
            // textGestureHorizontal
            // 
            this.textGestureHorizontal.Location = new System.Drawing.Point(170, 257);
            this.textGestureHorizontal.Name = "textGestureHorizontal";
            this.textGestureHorizontal.Size = new System.Drawing.Size(100, 20);
            this.textGestureHorizontal.TabIndex = 19;
            this.textGestureHorizontal.Text = "5";
            // 
            // labelMoveToleranceVertical
            // 
            this.labelMoveToleranceVertical.AutoSize = true;
            this.labelMoveToleranceVertical.Location = new System.Drawing.Point(21, 234);
            this.labelMoveToleranceVertical.Name = "labelMoveToleranceVertical";
            this.labelMoveToleranceVertical.Size = new System.Drawing.Size(143, 13);
            this.labelMoveToleranceVertical.TabIndex = 18;
            this.labelMoveToleranceVertical.Text = "movement tolerance vertical:";
            // 
            // textMoveToleranceVertical
            // 
            this.textMoveToleranceVertical.Location = new System.Drawing.Point(170, 231);
            this.textMoveToleranceVertical.Name = "textMoveToleranceVertical";
            this.textMoveToleranceVertical.Size = new System.Drawing.Size(100, 20);
            this.textMoveToleranceVertical.TabIndex = 17;
            this.textMoveToleranceVertical.Text = "5";
            // 
            // labelMoveToleranceHorizontal
            // 
            this.labelMoveToleranceHorizontal.AutoSize = true;
            this.labelMoveToleranceHorizontal.Location = new System.Drawing.Point(10, 208);
            this.labelMoveToleranceHorizontal.Name = "labelMoveToleranceHorizontal";
            this.labelMoveToleranceHorizontal.Size = new System.Drawing.Size(154, 13);
            this.labelMoveToleranceHorizontal.TabIndex = 16;
            this.labelMoveToleranceHorizontal.Text = "movement tolerance horizontal:";
            // 
            // textMoveToleranceHorizontal
            // 
            this.textMoveToleranceHorizontal.Location = new System.Drawing.Point(170, 205);
            this.textMoveToleranceHorizontal.Name = "textMoveToleranceHorizontal";
            this.textMoveToleranceHorizontal.Size = new System.Drawing.Size(100, 20);
            this.textMoveToleranceHorizontal.TabIndex = 15;
            this.textMoveToleranceHorizontal.Text = "5";
            // 
            // richTextConsole
            // 
            this.richTextConsole.Location = new System.Drawing.Point(6, 24);
            this.richTextConsole.Name = "richTextConsole";
            this.richTextConsole.Size = new System.Drawing.Size(258, 118);
            this.richTextConsole.TabIndex = 24;
            this.richTextConsole.Text = "";
            // 
            // groupConfiguration
            // 
            this.groupConfiguration.Controls.Add(this.labelGestureVertical);
            this.groupConfiguration.Controls.Add(this.textGestureVertical);
            this.groupConfiguration.Controls.Add(this.labelGestureHorizontal);
            this.groupConfiguration.Controls.Add(this.labelMoveToleranceHorizontal);
            this.groupConfiguration.Controls.Add(this.textGestureHorizontal);
            this.groupConfiguration.Controls.Add(this.labelMoveToleranceVertical);
            this.groupConfiguration.Controls.Add(this.textMoveToleranceVertical);
            this.groupConfiguration.Controls.Add(this.textMoveToleranceHorizontal);
            this.groupConfiguration.Controls.Add(this.labelSimilarPosRight);
            this.groupConfiguration.Controls.Add(this.textSimilarPosRight);
            this.groupConfiguration.Controls.Add(this.labelSimilarPosLeft);
            this.groupConfiguration.Controls.Add(this.textSimilarPosLeft);
            this.groupConfiguration.Controls.Add(this.labelSimilarPosStraight);
            this.groupConfiguration.Controls.Add(this.textSimilarPosStraight);
            this.groupConfiguration.Controls.Add(this.labelHaarStepHeight);
            this.groupConfiguration.Controls.Add(this.textHaarStepHeight);
            this.groupConfiguration.Controls.Add(this.labelHaarStepWidth);
            this.groupConfiguration.Controls.Add(this.textHaarStepWidth);
            this.groupConfiguration.Controls.Add(this.labelHaarHeight);
            this.groupConfiguration.Controls.Add(this.textHaarHeight);
            this.groupConfiguration.Controls.Add(this.labelHaarSingleWidth);
            this.groupConfiguration.Controls.Add(this.textHaarSingleWidth);
            this.groupConfiguration.Location = new System.Drawing.Point(12, 12);
            this.groupConfiguration.Name = "groupConfiguration";
            this.groupConfiguration.Size = new System.Drawing.Size(290, 318);
            this.groupConfiguration.TabIndex = 25;
            this.groupConfiguration.TabStop = false;
            this.groupConfiguration.Text = "Configuration:";
            // 
            // groupConsole
            // 
            this.groupConsole.Controls.Add(this.richTextConsole);
            this.groupConsole.Location = new System.Drawing.Point(318, 12);
            this.groupConsole.Name = "groupConsole";
            this.groupConsole.Size = new System.Drawing.Size(270, 148);
            this.groupConsole.TabIndex = 26;
            this.groupConsole.TabStop = false;
            this.groupConsole.Text = "Console:";
            // 
            // buttonStop
            // 
            this.buttonStop.Enabled = false;
            this.buttonStop.Location = new System.Drawing.Point(463, 217);
            this.buttonStop.Name = "buttonStop";
            this.buttonStop.Size = new System.Drawing.Size(119, 41);
            this.buttonStop.TabIndex = 27;
            this.buttonStop.Text = "Stop";
            this.buttonStop.UseVisualStyleBackColor = true;
            this.buttonStop.Click += new System.EventHandler(this.buttonStop_Click);
            // 
            // labelGestureText1
            // 
            this.labelGestureText1.AutoSize = true;
            this.labelGestureText1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.labelGestureText1.Location = new System.Drawing.Point(336, 168);
            this.labelGestureText1.Name = "labelGestureText1";
            this.labelGestureText1.Size = new System.Drawing.Size(100, 20);
            this.labelGestureText1.TabIndex = 29;
            this.labelGestureText1.Text = "Wykryto gest";
            this.labelGestureText1.Visible = false;
            // 
            // labelGestureDirection
            // 
            this.labelGestureDirection.AutoSize = true;
            this.labelGestureDirection.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.labelGestureDirection.ForeColor = System.Drawing.Color.Orange;
            this.labelGestureDirection.Location = new System.Drawing.Point(437, 191);
            this.labelGestureDirection.Name = "labelGestureDirection";
            this.labelGestureDirection.Size = new System.Drawing.Size(41, 20);
            this.labelGestureDirection.TabIndex = 28;
            this.labelGestureDirection.Text = "lewo";
            this.labelGestureDirection.Visible = false;
            // 
            // labelGestureText2
            // 
            this.labelGestureText2.AutoSize = true;
            this.labelGestureText2.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.labelGestureText2.Location = new System.Drawing.Point(416, 191);
            this.labelGestureText2.Name = "labelGestureText2";
            this.labelGestureText2.Size = new System.Drawing.Size(20, 20);
            this.labelGestureText2.TabIndex = 30;
            this.labelGestureText2.Text = "w";
            this.labelGestureText2.Visible = false;
            // 
            // labelGestureRecognitor
            // 
            this.labelGestureRecognitor.AutoSize = true;
            this.labelGestureRecognitor.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.labelGestureRecognitor.ForeColor = System.Drawing.Color.Orange;
            this.labelGestureRecognitor.Location = new System.Drawing.Point(437, 168);
            this.labelGestureRecognitor.Name = "labelGestureRecognitor";
            this.labelGestureRecognitor.Size = new System.Drawing.Size(80, 20);
            this.labelGestureRecognitor.TabIndex = 23;
            this.labelGestureRecognitor.Text = "dwa palce";
            this.labelGestureRecognitor.Visible = false;
            // 
            // textApp
            // 
            this.textApp.Location = new System.Drawing.Point(403, 295);
            this.textApp.Name = "textApp";
            this.textApp.Size = new System.Drawing.Size(179, 20);
            this.textApp.TabIndex = 37;
            this.textApp.Text = "IrfanView";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(306, 298);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(91, 13);
            this.label2.TabIndex = 36;
            this.label2.Text = "Application name:";
            // 
            // labelFileProp
            // 
            this.labelFileProp.AutoSize = true;
            this.labelFileProp.Location = new System.Drawing.Point(337, 269);
            this.labelFileProp.Name = "labelFileProp";
            this.labelFileProp.Size = new System.Drawing.Size(56, 13);
            this.labelFileProp.TabIndex = 42;
            this.labelFileProp.Text = "Config file:";
            // 
            // textFileProp
            // 
            this.textFileProp.Location = new System.Drawing.Point(403, 266);
            this.textFileProp.Name = "textFileProp";
            this.textFileProp.Size = new System.Drawing.Size(179, 20);
            this.textFileProp.TabIndex = 43;
            this.textFileProp.Text = "configGesture.prop";
            // 
            // buttonInfo
            // 
            this.buttonInfo.Location = new System.Drawing.Point(12, 338);
            this.buttonInfo.Name = "buttonInfo";
            this.buttonInfo.Size = new System.Drawing.Size(42, 19);
            this.buttonInfo.TabIndex = 41;
            this.buttonInfo.Text = "?";
            this.buttonInfo.UseVisualStyleBackColor = true;
            this.buttonInfo.Click += new System.EventHandler(this.buttonInfo_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(600, 371);
            this.Controls.Add(this.textFileProp);
            this.Controls.Add(this.labelFileProp);
            this.Controls.Add(this.buttonInfo);
            this.Controls.Add(this.textApp);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.labelGestureText2);
            this.Controls.Add(this.labelGestureRecognitor);
            this.Controls.Add(this.labelGestureText1);
            this.Controls.Add(this.labelGestureDirection);
            this.Controls.Add(this.buttonStop);
            this.Controls.Add(this.groupConsole);
            this.Controls.Add(this.groupConfiguration);
            this.Controls.Add(this.buttonRun);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "Form1";
            this.Text = "Wykrywanie gestów dłoni";
            this.groupConfiguration.ResumeLayout(false);
            this.groupConfiguration.PerformLayout();
            this.groupConsole.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonRun;
        private System.Windows.Forms.TextBox textHaarSingleWidth;
        private System.Windows.Forms.Label labelHaarSingleWidth;
        private System.Windows.Forms.Label labelHaarHeight;
        private System.Windows.Forms.TextBox textHaarHeight;
        private System.Windows.Forms.Label labelHaarStepWidth;
        private System.Windows.Forms.TextBox textHaarStepWidth;
        private System.Windows.Forms.Label labelHaarStepHeight;
        private System.Windows.Forms.TextBox textHaarStepHeight;
        private System.Windows.Forms.Label labelSimilarPosStraight;
        private System.Windows.Forms.TextBox textSimilarPosStraight;
        private System.Windows.Forms.Label labelSimilarPosLeft;
        private System.Windows.Forms.TextBox textSimilarPosLeft;
        private System.Windows.Forms.Label labelSimilarPosRight;
        private System.Windows.Forms.TextBox textSimilarPosRight;
        private System.Windows.Forms.Label labelGestureVertical;
        private System.Windows.Forms.TextBox textGestureVertical;
        private System.Windows.Forms.Label labelGestureHorizontal;
        private System.Windows.Forms.TextBox textGestureHorizontal;
        private System.Windows.Forms.Label labelMoveToleranceVertical;
        private System.Windows.Forms.TextBox textMoveToleranceVertical;
        private System.Windows.Forms.Label labelMoveToleranceHorizontal;
        private System.Windows.Forms.TextBox textMoveToleranceHorizontal;
        private System.Windows.Forms.RichTextBox richTextConsole;
        private System.Windows.Forms.GroupBox groupConfiguration;
        private System.Windows.Forms.GroupBox groupConsole;
        private System.Windows.Forms.Button buttonStop;
        private System.Windows.Forms.Label labelGestureText1;
        private System.Windows.Forms.Label labelGestureDirection;
        private System.Windows.Forms.Label labelGestureText2;
        private System.Windows.Forms.Label labelGestureRecognitor;
        private System.Windows.Forms.TextBox textApp;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label labelFileProp;
        private System.Windows.Forms.TextBox textFileProp;
        private System.Windows.Forms.Button buttonInfo;
    }
}

