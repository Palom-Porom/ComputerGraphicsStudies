namespace RGB_HSV_Lab
{
    partial class HSV_form
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
            this.pictureBox = new System.Windows.Forms.PictureBox();
            this.h_trackBar = new System.Windows.Forms.TrackBar();
            this.s_trackBar = new System.Windows.Forms.TrackBar();
            this.v_trackBar = new System.Windows.Forms.TrackBar();
            this.save_button = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.h_trackBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.s_trackBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.v_trackBar)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBox
            // 
            this.pictureBox.Image = global::RGB_HSV_Lab.Properties.Resources.fruit_test_image;
            this.pictureBox.Location = new System.Drawing.Point(2, 12);
            this.pictureBox.Name = "pictureBox";
            this.pictureBox.Size = new System.Drawing.Size(800, 350);
            this.pictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox.TabIndex = 2;
            this.pictureBox.TabStop = false;
            // 
            // h_trackBar
            // 
            this.h_trackBar.Location = new System.Drawing.Point(12, 393);
            this.h_trackBar.Maximum = 360;
            this.h_trackBar.Name = "h_trackBar";
            this.h_trackBar.Size = new System.Drawing.Size(206, 45);
            this.h_trackBar.TabIndex = 3;
            this.h_trackBar.ValueChanged += new System.EventHandler(this.trackBar_ValueChanged);
            // 
            // s_trackBar
            // 
            this.s_trackBar.Location = new System.Drawing.Point(224, 393);
            this.s_trackBar.Maximum = 100;
            this.s_trackBar.Minimum = -100;
            this.s_trackBar.Name = "s_trackBar";
            this.s_trackBar.Size = new System.Drawing.Size(206, 45);
            this.s_trackBar.TabIndex = 4;
            this.s_trackBar.ValueChanged += new System.EventHandler(this.trackBar_ValueChanged);
            // 
            // v_trackBar
            // 
            this.v_trackBar.Location = new System.Drawing.Point(436, 393);
            this.v_trackBar.Maximum = 100;
            this.v_trackBar.Minimum = -100;
            this.v_trackBar.Name = "v_trackBar";
            this.v_trackBar.Size = new System.Drawing.Size(206, 45);
            this.v_trackBar.TabIndex = 5;
            this.v_trackBar.ValueChanged += new System.EventHandler(this.trackBar_ValueChanged);
            // 
            // save_button
            // 
            this.save_button.Location = new System.Drawing.Point(674, 393);
            this.save_button.Name = "save_button";
            this.save_button.Size = new System.Drawing.Size(114, 45);
            this.save_button.TabIndex = 6;
            this.save_button.Text = "Save changes";
            this.save_button.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(83, 377);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(63, 13);
            this.label1.TabIndex = 7;
            this.label1.Text = "Hue [0;360]";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(300, 365);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(79, 13);
            this.label2.TabIndex = 8;
            this.label2.Text = "Saturation [0;1]";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(516, 365);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(58, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "Value [0;1]";
            // 
            // HSV_form
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.save_button);
            this.Controls.Add(this.v_trackBar);
            this.Controls.Add(this.s_trackBar);
            this.Controls.Add(this.h_trackBar);
            this.Controls.Add(this.pictureBox);
            this.Name = "HSV_form";
            this.Text = "HSV_form";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.h_trackBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.s_trackBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.v_trackBar)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox;
        private System.Windows.Forms.TrackBar h_trackBar;
        private System.Windows.Forms.TrackBar s_trackBar;
        private System.Windows.Forms.TrackBar v_trackBar;
        private System.Windows.Forms.Button save_button;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
    }
}