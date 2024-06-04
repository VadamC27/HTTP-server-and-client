using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Client
{
    public partial class Form1 : Form
    {
        private Socket clientSocket;
        public Form1()
        {
            InitializeComponent();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e) //GET BUTTON
        {
            ConnectAndSend("GET");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            ConnectAndSend("HEAD");
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void ConnectAndSend(string method)
        {
            if(textBoxAddress.Text.Length == 0)
            {
                return;
            }
            string serverAddress = textBoxAddress.Text;
            int serverPort = 8080;
            if (textBoxPort.Text != null)
            {
                serverPort = Int32.Parse(textBoxPort.Text);
            }
      
            try
            {
                Uri uri = new Uri(serverAddress);
                string path = uri.PathAndQuery;

                IPAddress ipAddress;

                // Check if the host is already an IP address
                if (IPAddress.TryParse(uri.Host, out ipAddress))
                {
                    clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                    clientSocket.Connect(new IPEndPoint(ipAddress, serverPort));
                }
                else
                {
                    // Resolve the host using DNS
                    IPHostEntry hostEntry = Dns.GetHostEntry(uri.Host);
                    ipAddress = hostEntry.AddressList[0];

                    clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                    clientSocket.Connect(new IPEndPoint(ipAddress, serverPort));
                }
                string request = $"{method} {path} HTTP/1.1\r\nHost: {serverAddress}\r\n\r\n";
                byte[] requestBytes = Encoding.ASCII.GetBytes(request);

                clientSocket.Send(requestBytes);

                byte[] buffer = new byte[4096];
                int bytesRead = clientSocket.Receive(buffer);
                string response = Encoding.ASCII.GetString(buffer, 0, bytesRead);

                richTextBoxAnwser.Clear();

                string[] lines = response.Split('\n');
           
                richTextBoxAnwser.SelectionColor = Color.Blue; 
                richTextBoxAnwser.AppendText(lines[0].TrimEnd('\r') + "\n");
                richTextBoxAnwser.SelectionColor = richTextBoxAnwser.ForeColor;

                for (int i = 1; i < lines.Length; i++)
                {
                    richTextBoxAnwser.AppendText(lines[i].TrimEnd('\r') + "\n");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message, "Error");
            }
            finally
            {
                if (clientSocket != null && clientSocket.Connected)
                    clientSocket.Close();
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void buttonPut_Click(object sender, EventArgs e)
        {

            if (textBoxAddress.Text.Length == 0)
            {
                return;
            }
            string serverAddress = textBoxAddress.Text;
            int serverPort = 8080;
            if (textBoxPort.Text != null)
            {
                serverPort = Int32.Parse(textBoxPort.Text);
            }

            try
            {
                Uri uri = new Uri(serverAddress);
                string path = uri.PathAndQuery;

                clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                clientSocket.Connect(new IPEndPoint(IPAddress.Parse(uri.Host), serverPort));

                string request = $"PUT {path} HTTP/1.1\r\nHost: {serverAddress}\r\n\r\n";
                request += textBoxPut.Text;
                byte[] requestBytes = Encoding.ASCII.GetBytes(request);

                clientSocket.Send(requestBytes);

                byte[] buffer = new byte[4096];
                int bytesRead = clientSocket.Receive(buffer);
                string response = Encoding.ASCII.GetString(buffer, 0, bytesRead);

                richTextBoxAnwser.Clear();

                string[] lines = response.Split('\n');

                richTextBoxAnwser.SelectionColor = Color.Blue;
                richTextBoxAnwser.AppendText(lines[0].TrimEnd('\r') + "\n");
                richTextBoxAnwser.SelectionColor = richTextBoxAnwser.ForeColor;

                for (int i = 1; i < lines.Length; i++)
                {
                    richTextBoxAnwser.AppendText(lines[i].TrimEnd('\r') + "\n");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message, "Error");
            }
            finally
            {
                if (clientSocket != null && clientSocket.Connected)
                    clientSocket.Close();
            }
        }

        private void buttonDelete_Click(object sender, EventArgs e)
        {
            if (textBoxAddress.Text.Length == 0)
            {
                return;
            }
            string serverAddress = textBoxAddress.Text;
            int serverPort = 8080;
            if (textBoxPort.Text != null)
            {
                serverPort = Int32.Parse(textBoxPort.Text);
            }

            try
            {
                Uri uri = new Uri(serverAddress);
                string path = uri.PathAndQuery;

                clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                clientSocket.Connect(new IPEndPoint(IPAddress.Parse(uri.Host), serverPort));

                string request = $"DELETE {path} HTTP/1.1\r\nHost: {serverAddress}\r\n\r\n";
                byte[] requestBytes = Encoding.ASCII.GetBytes(request);

                clientSocket.Send(requestBytes);

                byte[] buffer = new byte[4096];
                int bytesRead = clientSocket.Receive(buffer);
                string response = Encoding.ASCII.GetString(buffer, 0, bytesRead);

                richTextBoxAnwser.Clear();

                string[] lines = response.Split('\n');

                richTextBoxAnwser.SelectionColor = Color.Blue;
                richTextBoxAnwser.AppendText(lines[0].TrimEnd('\r') + "\n");
                richTextBoxAnwser.SelectionColor = richTextBoxAnwser.ForeColor;

                for (int i = 1; i < lines.Length; i++)
                {
                    richTextBoxAnwser.AppendText(lines[i].TrimEnd('\r') + "\n");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message, "Error");
            }
            finally
            {
                if (clientSocket != null && clientSocket.Connected)
                    clientSocket.Close();
            }
        }
    }
}
