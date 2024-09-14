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
using System.Diagnostics.Eventing.Reader;

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
        public GraphForm _graphForm;

        public Form1()
        {
            InitializeComponent();
            g = this.CreateGraphics();
            //this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseClick);

            comboBox1.SelectedIndex = 1;

            _graphForm = new GraphForm();
            AddOwnedForm(_graphForm);
        }
         
        private void PrintGraphButton_Click (object sender, MouseEventArgs e)
        {
            _graphForm._function = comboBox1.Text;
            _graphForm._stepRoundModifier = 1;
            if (double.TryParse(minVal_TextBox.Text, out _graphForm._minValue))
            {
                _graphForm._stepRoundModifier = Math.Max(_graphForm._stepRoundModifier,
                    minVal_TextBox.Text.SkipWhile(c => c != ',').Skip(1).Count());
            }
            else
                _graphForm._minValue = 0;
            if (double.TryParse(maxVal_TextBox.Text, out _graphForm._maxValue))
            {
                _graphForm._stepRoundModifier = Math.Max(_graphForm._stepRoundModifier,
                    maxVal_TextBox.Text.SkipWhile(c => c != '.').Skip(1).Count());
            }
            else
                _graphForm._maxValue = 10;
            _graphForm._stepRoundModifier = Math.Pow(10, _graphForm._stepRoundModifier);
            _graphForm.ShowDialog();
        }

        //private void Form1_MouseClick(object sender, MouseEventArgs e)
        //{
        //    Pen pen = new Pen(Color.SlateBlue);
        //    SolidBrush solid = new SolidBrush(Color.Red);
        //    g.FillEllipse(solid, e.X, e.Y, 5, 5);
        //    g.DrawEllipse(pen, e.X, e.Y, 5, 5);

        //    solid.Dispose();
        //    pen.Dispose();
        //}

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
