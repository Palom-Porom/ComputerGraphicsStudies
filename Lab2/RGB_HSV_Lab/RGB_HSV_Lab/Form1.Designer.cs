namespace RGB_HSV_Lab
{
    partial class Form1
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.desaturate_button = new System.Windows.Forms.Button();
            this.split_channels_button = new System.Windows.Forms.Button();
            this.change_hsv_button = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBox2
            // 
            this.pictureBox2.Image = global::RGB_HSV_Lab.Properties.Resources.fruit_test_image;
            this.pictureBox2.Location = new System.Drawing.Point(1, 12);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(800, 350);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox2.TabIndex = 1;
            this.pictureBox2.TabStop = false;
            // 
            // desaturate_button
            // 
            this.desaturate_button.Location = new System.Drawing.Point(79, 390);
            this.desaturate_button.Name = "desaturate_button";
            this.desaturate_button.Size = new System.Drawing.Size(140, 48);
            this.desaturate_button.TabIndex = 2;
            this.desaturate_button.Text = "Desaturate picture";
            this.desaturate_button.UseVisualStyleBackColor = true;
            this.desaturate_button.Click += new System.EventHandler(this.desaturate_button_Click);
            // 
            // split_channels_button
            // 
            this.split_channels_button.Location = new System.Drawing.Point(330, 390);
            this.split_channels_button.Name = "split_channels_button";
            this.split_channels_button.Size = new System.Drawing.Size(140, 48);
            this.split_channels_button.TabIndex = 3;
            this.split_channels_button.Text = "Split to color channels";
            this.split_channels_button.UseVisualStyleBackColor = true;
            // 
            // change_hsv_button
            // 
            this.change_hsv_button.Location = new System.Drawing.Point(569, 390);
            this.change_hsv_button.Name = "change_hsv_button";
            this.change_hsv_button.Size = new System.Drawing.Size(140, 48);
            this.change_hsv_button.TabIndex = 4;
            this.change_hsv_button.Text = "Change with HSV";
            this.change_hsv_button.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.change_hsv_button);
            this.Controls.Add(this.split_channels_button);
            this.Controls.Add(this.desaturate_button);
            this.Controls.Add(this.pictureBox2);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.Button desaturate_button;
        private System.Windows.Forms.Button split_channels_button;
        private System.Windows.Forms.Button change_hsv_button;
    }
}

