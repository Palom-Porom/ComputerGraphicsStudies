﻿using System;
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
    public partial class Form1 : Form
    {
        FormDesaturated _formDesaturated;
        FormSplittedChanels _formSplottedChannels;

        public Form1()
        {
            InitializeComponent();

            _formDesaturated = new FormDesaturated();
            AddOwnedForm(_formDesaturated);
            _formSplottedChannels = new FormSplittedChanels();
            AddOwnedForm(_formSplottedChannels);
        }

        private void desaturate_button_Click(object sender, EventArgs e)
        {
            _formDesaturated.ShowDialog();
        }

        private void split_channels_button_Click(object sender, EventArgs e)
        {
            _formSplottedChannels.ShowDialog();
        }
    }
}
