namespace BuildGraphicWindow
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
            this.DrawButton = new System.Windows.Forms.Button();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.minVal_TextBox = new System.Windows.Forms.TextBox();
            this.maxVal_TextBox = new System.Windows.Forms.TextBox();
            this.InfoLabel_function = new System.Windows.Forms.Label();
            this.InfoLabel_maxValue = new System.Windows.Forms.Label();
            this.InfoLabel_minValue = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // DrawButton
            // 
            this.DrawButton.Location = new System.Drawing.Point(293, 393);
            this.DrawButton.Name = "DrawButton";
            this.DrawButton.Size = new System.Drawing.Size(190, 45);
            this.DrawButton.TabIndex = 0;
            this.DrawButton.Text = "Draw Graphic";
            this.DrawButton.UseVisualStyleBackColor = true;
            this.DrawButton.MouseClick += new System.Windows.Forms.MouseEventHandler(this.PrintGraphButton_Click);
            // 
            // comboBox1
            // 
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Items.AddRange(new object[] {
            "Sin(x)",
            "Cos(x)",
            "Pow(x, 2)",
            "x",
            "x + 1",
            "x/2-5",
            "(x+2)*2"});
            this.comboBox1.Location = new System.Drawing.Point(293, 341);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(190, 21);
            this.comboBox1.TabIndex = 2;
            // 
            // minVal_TextBox
            // 
            this.minVal_TextBox.Location = new System.Drawing.Point(187, 342);
            this.minVal_TextBox.Name = "minVal_TextBox";
            this.minVal_TextBox.Size = new System.Drawing.Size(100, 20);
            this.minVal_TextBox.TabIndex = 3;
            // 
            // maxVal_TextBox
            // 
            this.maxVal_TextBox.Location = new System.Drawing.Point(489, 341);
            this.maxVal_TextBox.Name = "maxVal_TextBox";
            this.maxVal_TextBox.Size = new System.Drawing.Size(100, 20);
            this.maxVal_TextBox.TabIndex = 4;
            // 
            // InfoLabel_function
            // 
            this.InfoLabel_function.AutoSize = true;
            this.InfoLabel_function.Location = new System.Drawing.Point(346, 312);
            this.InfoLabel_function.Name = "InfoLabel_function";
            this.InfoLabel_function.Size = new System.Drawing.Size(70, 13);
            this.InfoLabel_function.TabIndex = 5;
            this.InfoLabel_function.Text = "Your function";
            // 
            // InfoLabel_maxValue
            // 
            this.InfoLabel_maxValue.AutoSize = true;
            this.InfoLabel_maxValue.Location = new System.Drawing.Point(486, 312);
            this.InfoLabel_maxValue.Name = "InfoLabel_maxValue";
            this.InfoLabel_maxValue.Size = new System.Drawing.Size(155, 13);
            this.InfoLabel_maxValue.TabIndex = 6;
            this.InfoLabel_maxValue.Text = "maxValue (use \',\' for float nums)";
            // 
            // InfoLabel_minValue
            // 
            this.InfoLabel_minValue.AutoSize = true;
            this.InfoLabel_minValue.Location = new System.Drawing.Point(132, 312);
            this.InfoLabel_minValue.Name = "InfoLabel_minValue";
            this.InfoLabel_minValue.Size = new System.Drawing.Size(152, 13);
            this.InfoLabel_minValue.TabIndex = 7;
            this.InfoLabel_minValue.Text = "minValue (use \',\' for float nums)";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.InfoLabel_minValue);
            this.Controls.Add(this.InfoLabel_maxValue);
            this.Controls.Add(this.InfoLabel_function);
            this.Controls.Add(this.maxVal_TextBox);
            this.Controls.Add(this.minVal_TextBox);
            this.Controls.Add(this.comboBox1);
            this.Controls.Add(this.DrawButton);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button DrawButton;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.TextBox minVal_TextBox;
        private System.Windows.Forms.TextBox maxVal_TextBox;
        private System.Windows.Forms.Label InfoLabel_function;
        private System.Windows.Forms.Label InfoLabel_maxValue;
        private System.Windows.Forms.Label InfoLabel_minValue;
    }
}

