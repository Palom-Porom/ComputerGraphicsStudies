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
using static System.Windows.Forms.AxHost;

namespace RGB_HSV_Lab
{
    public partial class HSV_form : Form
    {
        Bitmap _bitmap;
        HSV_Bitmap _hsv_map;

        public HSV_form()
        {
            InitializeComponent();

            _bitmap = new Bitmap(pictureBox.Image);
            _hsv_map = new HSV_Bitmap(_bitmap);
            
            last_h = h_trackBar.Value;
            last_s = s_trackBar.Value;
            last_v = v_trackBar.Value;
        }


        int last_h;
        int last_s;
        int last_v;
        private void trackBar_ValueChanged(object sender, EventArgs e)
        {
            _hsv_map.ChangeHSL(ref _bitmap, (h_trackBar.Value - last_h, (s_trackBar.Value - last_s) / 100f, (s_trackBar.Value - last_s) / 100f));
            pictureBox.Image = _bitmap;
            last_h = h_trackBar.Value;
            last_s = s_trackBar.Value;
            last_v = v_trackBar.Value;
        }


        public class HSV_Bitmap
        {
            private int[,] h_arr;
            private float[,] s_arr, v_arr;


            public HSV_Bitmap(Bitmap rgb_bitmap)
            {
                h_arr = new int[rgb_bitmap.Width, rgb_bitmap.Height];
                s_arr = new float[rgb_bitmap.Width, rgb_bitmap.Height];
                v_arr = new float[rgb_bitmap.Width, rgb_bitmap.Height];
                for (int i = 0; i < rgb_bitmap.Width; i++)
                {
                    for (int j = 0; j < rgb_bitmap.Height; j++)
                    {
                        (h_arr[i,j], s_arr[i, j], v_arr[i, j]) = rgb2hsv(rgb_bitmap.GetPixel(i, j));
                    }
                }
            }

            public void ChangeHSL(ref Bitmap rgb_bitmap, (int, float, float) increment)
            {
                for (int i = 0; i < rgb_bitmap.Width; i++)
                    for (int j = 0; j < rgb_bitmap.Height; j++)
                    {
                        h_arr[i, j] = (h_arr[i, j] + increment.Item1) % 360;
                        s_arr[i, j] = s_arr[i, j] + increment.Item2;
                        v_arr[i, j] = v_arr[i, j] + increment.Item3;

                        int h = h_arr[i, j];
                        float s = clamp(s_arr[i, j], 0f, 1f);
                        float v = clamp(v_arr[i, j], 0f, 1f);

                        int hi = (int)(h / 60f) % 6;
                        float f = h / 60f - (int)(h / 60f);
                        float p = v * (1 - s);
                        float q = v * (1 - f * s);
                        float t = v * (1 - (1 - f) * s);

                        float r = 0, g = 0, b = 0;
                        switch (hi)
                        {
                            case 0:
                                r = v; g = t; b = p; break;
                            case 1:
                                r = q; g = v; b = p; break;
                            case 2:
                                r = p; g = v; b = t; break;
                            case 3:
                                r = p; g = q; b = v; break;
                            case 4:
                                r = t; g = p; b = v; break;
                            case 5:
                                r = v; g = p; b = q; break;
                        }

                        rgb_bitmap.SetPixel(i, j, Color.FromArgb((byte)(r * 255), (byte)(g * 255), (byte)(b * 255)));
                    }
                
            }

            private float clamp (float value, float min, float max)
            {
                if (value < min)
                {
                    return min;
                }
                else if (value > max)
                {
                    return max;
                }

                return value;
            }

            private (int, float, float) rgb2hsv (Color rgb)
            {
                int h;
                float s, v;
                (float r, float g, float b) = (rgb.R / 256f, rgb.G / 256f, rgb.B / 256f);
                float max = Math.Max(r, Math.Max(g, b));
                float min = Math.Min(r, Math.Min(g, b));

                //hue
                if (max == min)
                    h = 0;
                else if (max == r)
                {
                    if (g >= b)
                        h = (int)(60 * (g - b) / (max - min));
                    else
                        h = (int)(60 * (g - b) / (max - min) + 360);
                }
                else if (max == g)
                    h = (int)(60 * (b - r) / (max - min) + 120);
                else
                    h = (int)(60 * (r - g) / (max - min) + 240);

                //value
                v = max;

                //saturation
                if (max == 0)
                    s = 0;
                else
                    s = 1 - min / max;

                return (h, s, v);
            }
            
        }

    }
}
