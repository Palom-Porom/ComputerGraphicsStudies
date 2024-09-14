using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using CodingSeb.ExpressionEvaluator;

namespace BuildGraphicWindow
{
    public partial class GraphForm : Form
    {
        public double _minValue;
        public double _maxValue;
        /// <summary> Depends on the max quantity of numbers after dot in _minValue and _maxValue </summary>
        public double _stepRoundModifier;
        private const double _stepRoundModifierY = 100;
        private double _dist;
        public string _function;
        Dictionary<string, double> _varDict;
        ExpressionEvaluator _expressionEvaluator;
        (double, double)[] _functionVals;
        private double _minFunctionVal;
        private double _maxFunctionVal;
        private double _distFunctionVal;
        private double _zeroVal;

        private Pen _gridPen;
        private Pen _graphPen;
        private Font _graphFont;
        private Brush _graphBrush;

        List<Label> xLabels, yLabels;

        public GraphForm()
        {
            InitializeComponent();
            _gridPen = new Pen(Color.Black, 2f);
            _graphPen = new Pen(Color.Red, 2f);
            _graphFont = new Font(Font, FontStyle.Bold);
            _graphBrush = new SolidBrush(Color.Black);

            _varDict = new Dictionary<string, double>() { { "x", 0} };
            _expressionEvaluator = new ExpressionEvaluator();
            _expressionEvaluator.Variables["x"] = 0;
        }

        private void GraphForm_Load(object sender, EventArgs e)
        {
            #region Calculate Vals

            _dist = _maxValue - _minValue;
            int numOfVals = (int)(_dist * 10 + 1);
            _functionVals = new (double, double)[numOfVals];
            _expressionEvaluator.Variables["x"] = _minValue;
            for (int i = 0; i < numOfVals; i++)
            {
                var curX = double.Parse(_expressionEvaluator.Variables["x"].ToString());
                _functionVals[i] = (curX, double.Parse(_expressionEvaluator.Evaluate(_function).ToString()));
                _expressionEvaluator.Variables["x"] = curX + 0.1;
            }
            _minFunctionVal = _functionVals.Select(x => x.Item2).Min();
            _maxFunctionVal = _functionVals.Select(x => x.Item2).Max();
            _distFunctionVal = _maxFunctionVal - _minFunctionVal;
            _expressionEvaluator.Variables["x"] = 0;
            _zeroVal = double.Parse(_expressionEvaluator.Evaluate(_function).ToString());
            //label2.Text = $"{_functionVals[0]}, {_functionVals[1]}, {_functionVals[2]}, {_functionVals[3]}, ... {_functionVals[_functionVals.Length - 1]}";

            #endregion

            #region Draw Graph



            #endregion
        }

        private void GraphForm_Paint(object sender, PaintEventArgs e)
        {
            Graphics _g = e.Graphics;
            _g.Clear(Color.White);

            #region Draw Graph

            Point[] graphPoints = _functionVals.Select(x => (ClientSize.Width * (x.Item1 - _minValue) / _dist, ClientSize.Height * ((_maxFunctionVal - x.Item2) / _distFunctionVal)))
                .Select(x => new Point((int)x.Item1, (int)x.Item2)).ToArray();
            _g.DrawCurve(_graphPen, graphPoints);

            #endregion

            #region Draw Grid

            int verticalLineX = (int)(ClientSize.Width * (0 - _minValue) / _dist);
            int horizontalLineY = (int)(ClientSize.Height * (_maxFunctionVal - 0) / _distFunctionVal);
            _g.DrawLine(_gridPen, new Point((int)(ClientSize.Width * (0 - _minValue) / _dist), 0),
                new Point ((int)(ClientSize.Width * (0 - _minValue) / _dist), ClientSize.Height));
            _g.DrawLine(_gridPen, new Point(0, (int)(ClientSize.Height * (_maxFunctionVal - 0) / _distFunctionVal)),
                new Point(ClientSize.Width, (int)(ClientSize.Height * (_maxFunctionVal - 0) / _distFunctionVal)));

            //========================
            // X
            {
                int aroundXLineIncrement = (_maxFunctionVal - _zeroVal) / _distFunctionVal > 0.5 ? -20 : 20;

                //Draw border values
                {
                    _g.DrawString(_minValue.ToString(), _graphFont, _graphBrush, new RectangleF(new Point(0, horizontalLineY + aroundXLineIncrement), new SizeF(40, 40)), new StringFormat { Alignment = StringAlignment.Center, Trimming = StringTrimming.EllipsisCharacter });
                    _g.DrawLine(_gridPen, new Point(0, horizontalLineY - aroundXLineIncrement / 3), new Point(0, horizontalLineY + aroundXLineIncrement / 3));

                    _g.DrawString(_maxValue.ToString(), _graphFont, _graphBrush, new RectangleF(new Point(ClientSize.Width - 40, horizontalLineY + aroundXLineIncrement), new SizeF(40, 40)), new StringFormat { Alignment = StringAlignment.Center, Trimming = StringTrimming.EllipsisCharacter });
                    _g.DrawLine(_gridPen, new Point(ClientSize.Width, horizontalLineY - aroundXLineIncrement / 3), new Point(ClientSize.Width, horizontalLineY + aroundXLineIncrement / 3));
                }

                double curLoopVal, curShownVal;
                double step = _dist * 80f / ClientSize.Width;
                //label2.Text = _stepRoundModifier.ToString();
                if (_minValue * _maxValue < 0)
                {
                    curLoopVal = step;
                    while (-curLoopVal > _minValue || curLoopVal < _maxValue)
                    {
                        if (curLoopVal < _maxValue)
                        {
                            curShownVal = Math.Truncate(curLoopVal * _stepRoundModifier) / _stepRoundModifier;
                            _g.DrawString(curShownVal.ToString(), _graphFont, _graphBrush,
                                new RectangleF(new Point((int)(ClientSize.Width * (curLoopVal - _minValue) / _dist) - 20, horizontalLineY + aroundXLineIncrement), new SizeF(40, 40)),
                                new StringFormat { Alignment = StringAlignment.Center, Trimming = StringTrimming.EllipsisCharacter });
                            _g.DrawLine(_gridPen, new Point((int)(ClientSize.Width * (curLoopVal - _minValue) / _dist), horizontalLineY - aroundXLineIncrement / 3),
                                new Point((int)(ClientSize.Width * (curLoopVal - _minValue) / _dist), horizontalLineY + aroundXLineIncrement / 3));
                        }
                        if (curLoopVal > _minValue)
                        {
                            curShownVal = Math.Truncate(-curLoopVal * _stepRoundModifier) / _stepRoundModifier;
                            _g.DrawString(curShownVal.ToString(), _graphFont, _graphBrush,
                                new RectangleF(new Point((int)(ClientSize.Width * (-curLoopVal - _minValue) / _dist) - 20, horizontalLineY + aroundXLineIncrement), new SizeF(40, 40)),
                                new StringFormat { Alignment = StringAlignment.Center, Trimming = StringTrimming.EllipsisCharacter });
                            _g.DrawLine(_gridPen, new Point((int)(ClientSize.Width * (-curLoopVal - _minValue) / _dist), horizontalLineY - aroundXLineIncrement / 3),
                                new Point((int)(ClientSize.Width * (-curLoopVal - _minValue) / _dist), horizontalLineY + aroundXLineIncrement / 3));
                        }
                        curLoopVal += step;
                    }
                }
                else
                {
                    curLoopVal = _minValue + step;
                    while (curLoopVal <= _maxValue)
                    {
                        curShownVal = Math.Truncate(curLoopVal * _stepRoundModifier) / _stepRoundModifier;
                        _g.DrawString(curShownVal.ToString(), _graphFont, _graphBrush,
                            new RectangleF(new Point((int)(ClientSize.Width * (curLoopVal - _minValue) / _dist) - 20, horizontalLineY + aroundXLineIncrement), new SizeF(40, 40)),
                            new StringFormat { Alignment = StringAlignment.Center });
                        _g.DrawLine(_gridPen, new Point((int)(ClientSize.Width * (curLoopVal - _minValue) / _dist), horizontalLineY - aroundXLineIncrement / 3),
                            new Point((int)(ClientSize.Width * (curLoopVal - _minValue) / _dist), horizontalLineY + aroundXLineIncrement / 3));
                        curLoopVal += step;
                    }
                }
            }

            //========================
            // Y
            {
                int aroundYLineIncrement = (0 - _minValue) / _dist > 0.5 ? -20 : 20;

                //Draw border values
                {
                    _g.DrawString((Math.Truncate(_maxFunctionVal * _stepRoundModifierY) / _stepRoundModifierY).ToString(), _graphFont, _graphBrush, new RectangleF(new Point(verticalLineX + aroundYLineIncrement, 0), new SizeF(40, 40)), new StringFormat { Alignment = StringAlignment.Center, Trimming = StringTrimming.EllipsisCharacter });
                    _g.DrawLine(_gridPen, new Point(verticalLineX - aroundYLineIncrement / 3, 0), new Point(verticalLineX + aroundYLineIncrement / 3, 0));

                    _g.DrawString((Math.Truncate(_minFunctionVal * _stepRoundModifierY) / _stepRoundModifierY).ToString(), _graphFont, _graphBrush, new RectangleF(new Point(verticalLineX + aroundYLineIncrement, ClientSize.Height - 20), new SizeF(40, 40)), new StringFormat { Alignment = StringAlignment.Center, Trimming = StringTrimming.EllipsisCharacter });
                    _g.DrawLine(_gridPen, new Point(verticalLineX - aroundYLineIncrement / 3, ClientSize.Height), new Point(verticalLineX + aroundYLineIncrement / 3, ClientSize.Height));
                }

                double curLoopVal, curShownVal;
                double step = _distFunctionVal * 80f / ClientSize.Height;
                //label2.Text = _stepRoundModifier.ToString();
                if (_minFunctionVal * _maxFunctionVal < 0)
                {
                    curLoopVal = step;
                    while (-curLoopVal > _minFunctionVal || curLoopVal < _maxFunctionVal)
                    {
                        if (curLoopVal < _maxValue)
                        {
                            curShownVal = Math.Truncate(curLoopVal * _stepRoundModifierY) / _stepRoundModifierY;
                            _g.DrawString(curShownVal.ToString(), _graphFont, _graphBrush,
                                new RectangleF(new Point(verticalLineX + aroundYLineIncrement, (int)(ClientSize.Height * (_maxFunctionVal - curLoopVal) / _distFunctionVal) - 20), new SizeF(40, 40)),
                                new StringFormat { Alignment = StringAlignment.Center, Trimming = StringTrimming.EllipsisCharacter });
                            _g.DrawLine(_gridPen, new Point(verticalLineX - aroundYLineIncrement / 3, (int)(ClientSize.Height * (_maxFunctionVal - curLoopVal) / _distFunctionVal)),
                                new Point(verticalLineX + aroundYLineIncrement / 3, (int)(ClientSize.Height * (_maxFunctionVal - curLoopVal) / _distFunctionVal)));
                        }
                        if (curLoopVal > _minValue)
                        {
                            curShownVal = Math.Truncate(-curLoopVal * _stepRoundModifierY) / _stepRoundModifierY;
                            _g.DrawString(curShownVal.ToString(), _graphFont, _graphBrush,
                                new RectangleF(new Point(verticalLineX + aroundYLineIncrement, (int)(ClientSize.Height * (_maxFunctionVal + curLoopVal) / _distFunctionVal) - 20), new SizeF(40, 40)),
                                new StringFormat { Alignment = StringAlignment.Center, Trimming = StringTrimming.EllipsisCharacter });
                            _g.DrawLine(_gridPen, new Point(verticalLineX - aroundYLineIncrement / 3, (int)(ClientSize.Height * (_maxFunctionVal + curLoopVal) / _distFunctionVal)),
                                new Point(verticalLineX + aroundYLineIncrement / 3, (int)(ClientSize.Height * (_maxFunctionVal + curLoopVal) / _distFunctionVal)));
                        }
                        curLoopVal += step;
                    }
                }
                else
                {
                    curLoopVal = _minValue + step;
                    while (curLoopVal <= _maxValue)
                    {
                        curShownVal = Math.Truncate(curLoopVal * _stepRoundModifierY) / _stepRoundModifierY;
                        _g.DrawString(curShownVal.ToString(), _graphFont, _graphBrush,
                            new RectangleF(new Point(verticalLineX + aroundYLineIncrement, (int)(ClientSize.Height * (_maxFunctionVal - curLoopVal) / _distFunctionVal) - 20), new SizeF(40, 40)),
                            new StringFormat { Alignment = StringAlignment.Center });
                        _g.DrawLine(_gridPen, new Point(verticalLineX - aroundYLineIncrement / 3, (int)(ClientSize.Height * (_maxFunctionVal - curLoopVal) / _distFunctionVal)),
                            new Point(verticalLineX + aroundYLineIncrement / 3, (int)(ClientSize.Height * (_maxFunctionVal - curLoopVal) / _distFunctionVal)));
                        curLoopVal += step;
                    }
                }
            }

            #endregion
        }

        private void GraphForm_ResizeEnd(object sender, EventArgs e)
        {
            //this.Invalidate();
        }
    }
}
