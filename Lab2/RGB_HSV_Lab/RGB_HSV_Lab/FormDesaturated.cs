using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using FastBitmap;

namespace RGB_HSV_Lab
{
    public partial class FormDesaturated : Form
    {
        public FormDesaturated()
        {
            InitializeComponent();

            Bitmap bitmap = new Bitmap(first_formula_pictureBox.Image);

            first_formula_pictureBox.Image = bitmap.Select(color =>
            {
                var avg = (byte)Math.Round((color.R * 0.299 + color.G * 0.587 + color.B * 0.144) / 3);
                return Color.FromArgb(avg, avg, avg);
            });
            second_formula_pictureBox.Image = bitmap.Select(color =>
            {
                var avg = (byte)Math.Round((color.R * 0.2126 + color.G * 0.7152 + color.B * 0.0722) / 3);
                return Color.FromArgb(avg, avg, avg);
            });
            difference_pictureBox.Image = bitmap.Select(color =>
            {
                var avg = (byte)Math.Round((color.R * 0.299 - color.R * 0.2126 + color.G * 0.7152 - color.G * 0.587 + color.B * 0.144 - color.B * 0.0722) / 3);
                return Color.FromArgb(avg, avg, avg);
            });
        }

        private void second_formula_pictureBox_Click(object sender, EventArgs e)
        {

        }
    }
}
