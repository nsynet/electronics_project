using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            try
            {
                foreach (string com in System.IO.Ports.SerialPort.GetPortNames()) //自动获取串行口名称
                    comboBoxComNum.Items.Add(com);
                //  cmbport.SelectedIndex = 0;
            }
            catch
            {
                MessageBox.Show("找不到通讯端口！");
            }
        }
        private void button1_Click(object sender, EventArgs e)
        {
            if (comboBoxComNum.SelectedIndex == -1)
            {
                MessageBox.Show("Please select COM");
            }
            else
            {
                if (serialPort1.IsOpen == false)
                {

                    serialPort1.PortName = comboBoxComNum.SelectedItem.ToString();
                    serialPort1.Open();
                    buttonComOpen.Text = "Close";
                }
                else
                {
                    serialPort1.Close();
                    buttonComOpen.Text = "Open";
                }
            }
        }

        Bitmap bmpZoom = null;  
        private void pictureBox1_MouseClick(object sender, MouseEventArgs e)
        {
            bmpZoom = new Bitmap(pictureBox1.Image);  
            Color color = bmpZoom.GetPixel(e.X, e.Y);
            labelColor.BackColor = color;
            String val = color.ToArgb().ToString("X");
            textBox1.Text = "R: " + color.R.ToString("X2");
            textBox2.Text = "G: " + color.G.ToString("X2");
            textBox3.Text = "B: " + color.B.ToString("X2");

            string str = "*"
                        + color.R.ToString("X2")
                        + color.G.ToString("X2")
                        + color.B.ToString("X2")
                        + "#";
            if (serialPort1.IsOpen == false)
            {
                
            }
            else
            {
                serialPort1.WriteLine(str);
            }            
        }

        private int rgb565FromColor(Color color)
        {
            int val = color.B >> 3 << 11;
            val |= color.G >> 2 << 5;
            val |= color.R >> 3;

            return val;
        }

        private int rgb565PFromColor(Color color)
        {
            int val = color.R >> 3 << 11;
            val |= color.G >> 2 << 5;
            val |= color.B >> 3;

            return val;
        }

        private void buttonOff_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}