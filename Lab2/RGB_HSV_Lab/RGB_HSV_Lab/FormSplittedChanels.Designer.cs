namespace RGB_HSV_Lab
{
    partial class FormSplittedChanels
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
            this.blue_pictureBox = new System.Windows.Forms.PictureBox();
            this.green_pictureBox = new System.Windows.Forms.PictureBox();
            this.red_pictureBox = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.blue_pictureBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.green_pictureBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.red_pictureBox)).BeginInit();
            this.SuspendLayout();
            // 
            // blue_pictureBox
            // 
            this.blue_pictureBox.Image = global::RGB_HSV_Lab.Properties.Resources.fruit_test_image;
            this.blue_pictureBox.Location = new System.Drawing.Point(1057, 12);
            this.blue_pictureBox.Name = "blue_pictureBox";
            this.blue_pictureBox.Size = new System.Drawing.Size(511, 382);
            this.blue_pictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.blue_pictureBox.TabIndex = 8;
            this.blue_pictureBox.TabStop = false;
            // 
            // green_pictureBox
            // 
            this.green_pictureBox.Image = global::RGB_HSV_Lab.Properties.Resources.fruit_test_image;
            this.green_pictureBox.Location = new System.Drawing.Point(528, 12);
            this.green_pictureBox.Name = "green_pictureBox";
            this.green_pictureBox.Size = new System.Drawing.Size(505, 382);
            this.green_pictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.green_pictureBox.TabIndex = 7;
            this.green_pictureBox.TabStop = false;
            // 
            // red_pictureBox
            // 
            this.red_pictureBox.Image = global::RGB_HSV_Lab.Properties.Resources.fruit_test_image;
            this.red_pictureBox.Location = new System.Drawing.Point(11, 12);
            this.red_pictureBox.Name = "red_pictureBox";
            this.red_pictureBox.Size = new System.Drawing.Size(492, 382);
            this.red_pictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.red_pictureBox.TabIndex = 6;
            this.red_pictureBox.TabStop = false;
            // 
            // FormSplittedChanels
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1569, 747);
            this.Controls.Add(this.blue_pictureBox);
            this.Controls.Add(this.green_pictureBox);
            this.Controls.Add(this.red_pictureBox);
            this.Name = "FormSplittedChanels";
            this.Text = "FormSplittedChanels";
            ((System.ComponentModel.ISupportInitialize)(this.blue_pictureBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.green_pictureBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.red_pictureBox)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox blue_pictureBox;
        private System.Windows.Forms.PictureBox green_pictureBox;
        private System.Windows.Forms.PictureBox red_pictureBox;
    }
}