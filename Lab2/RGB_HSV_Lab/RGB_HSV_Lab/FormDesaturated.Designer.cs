namespace RGB_HSV_Lab
{
    partial class FormDesaturated
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
            this.difference_pictureBox = new System.Windows.Forms.PictureBox();
            this.second_formula_pictureBox = new System.Windows.Forms.PictureBox();
            this.first_formula_pictureBox = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.difference_pictureBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.second_formula_pictureBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.first_formula_pictureBox)).BeginInit();
            this.SuspendLayout();
            // 
            // difference_pictureBox
            // 
            this.difference_pictureBox.Image = global::RGB_HSV_Lab.Properties.Resources.fruit_test_image;
            this.difference_pictureBox.Location = new System.Drawing.Point(1058, 12);
            this.difference_pictureBox.Name = "difference_pictureBox";
            this.difference_pictureBox.Size = new System.Drawing.Size(511, 382);
            this.difference_pictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.difference_pictureBox.TabIndex = 5;
            this.difference_pictureBox.TabStop = false;
            // 
            // second_formula_pictureBox
            // 
            this.second_formula_pictureBox.Image = global::RGB_HSV_Lab.Properties.Resources.fruit_test_image;
            this.second_formula_pictureBox.Location = new System.Drawing.Point(529, 12);
            this.second_formula_pictureBox.Name = "second_formula_pictureBox";
            this.second_formula_pictureBox.Size = new System.Drawing.Size(505, 382);
            this.second_formula_pictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.second_formula_pictureBox.TabIndex = 4;
            this.second_formula_pictureBox.TabStop = false;
            this.second_formula_pictureBox.Click += new System.EventHandler(this.second_formula_pictureBox_Click);
            // 
            // first_formula_pictureBox
            // 
            this.first_formula_pictureBox.Image = global::RGB_HSV_Lab.Properties.Resources.fruit_test_image;
            this.first_formula_pictureBox.Location = new System.Drawing.Point(12, 12);
            this.first_formula_pictureBox.Name = "first_formula_pictureBox";
            this.first_formula_pictureBox.Size = new System.Drawing.Size(492, 382);
            this.first_formula_pictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.first_formula_pictureBox.TabIndex = 3;
            this.first_formula_pictureBox.TabStop = false;
            // 
            // FormDesaturated
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1581, 728);
            this.Controls.Add(this.difference_pictureBox);
            this.Controls.Add(this.second_formula_pictureBox);
            this.Controls.Add(this.first_formula_pictureBox);
            this.Name = "FormDesaturated";
            this.Text = "FormDesaturated";
            ((System.ComponentModel.ISupportInitialize)(this.difference_pictureBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.second_formula_pictureBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.first_formula_pictureBox)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox difference_pictureBox;
        private System.Windows.Forms.PictureBox second_formula_pictureBox;
        private System.Windows.Forms.PictureBox first_formula_pictureBox;
    }
}