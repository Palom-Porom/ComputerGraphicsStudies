using FastBitmap;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RGB_HSV_Lab
{
    public partial class FormSplittedChanels : Form
    {
        public FormSplittedChanels()
        {
            InitializeComponent();

            Bitmap bitmap = new Bitmap(red_pictureBox.Image);

            red_pictureBox.Image = bitmap.Select(color => Color.FromArgb(color.R, 0, 0));
            green_pictureBox.Image = bitmap.Select(color => Color.FromArgb(0, color.G, 0));
            blue_pictureBox.Image = bitmap.Select(color => Color.FromArgb(0, 0, color.B));
        }
    }
}
