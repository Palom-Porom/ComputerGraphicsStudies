using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using CodingSeb.ExpressionEvaluator;

namespace BuildGraphicWindow
{
    public partial class Form1 : Form
    {
        //public Form1()
        //{
        //    InitializeComponent();
        //}


        //===========================================
        //===========================================

        private Graphics g;

        public Form1()
        {
            InitializeComponent();
            g = this.CreateGraphics();
            //this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseClick);
        }
         
        private void Form1_MouseClick(object sender, MouseEventArgs e)
        {
            Pen pen = new Pen(Color.SlateBlue);
            SolidBrush solid = new SolidBrush(Color.Red);
            g.FillEllipse(solid, e.X, e.Y, 5, 5);
            g.DrawEllipse(pen, e.X, e.Y, 5, 5);

            solid.Dispose();
            pen.Dispose();
        }

        //===========================================
        //===========================================

        //private Graphics g;
        //private Image im;
        //private PictureBox pictureBox1;

        //public Form1()
        //{
        //    InitializeComponent();
        //    this.pictureBox1 = new PictureBox();
        //    pictureBox1.Image = new Bitmap(pictureBox1.Width, pictureBox1.Height);
        //    g = Graphics.FromImage(pictureBox1.Image);
        //    g.Clear(Color.White);

        //    //this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseClick);
        //    //this.MouseDoubleClick += button1_Click;
        //}

        //private void Form1_MouseClick(object sender, MouseEventArgs e)
        //{
        //    Pen pen = new Pen(Color.SlateBlue);
        //    SolidBrush solid = new SolidBrush(Color.Red);
        //    g.FillEllipse(solid, e.X, e.Y, 5, 5);
        //    g.DrawEllipse(pen, e.X, e.Y, 5, 5);

        //    solid.Dispose();
        //    pen.Dispose();
        //    pictureBox1.Invalidate();

        //}

        //private void button1_Click(object sender, EventArgs e)
        //{
        //    pictureBox1.Image.Save("image1.png");
        //}
    }
}
