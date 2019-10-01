namespace server
{
    partial class ChatForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        void transButton_Click(object sender, System.EventArgs e) { }
        void outputBox_TextChanged(object sender, System.EventArgs e) { }
        void inputBox_TextChanged(object sender, System.EventArgs e) { }

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.transButton = new System.Windows.Forms.Button();
            this.fontDialog1 = new System.Windows.Forms.FontDialog();
            this.outputBox = new System.Windows.Forms.RichTextBox();
            this.inputBox = new System.Windows.Forms.RichTextBox();
            this.SuspendLayout();
            // 
            // transButton
            // 
            this.transButton.Location = new System.Drawing.Point(637, 396);
            this.transButton.Name = "transButton";
            this.transButton.Size = new System.Drawing.Size(146, 41);
            this.transButton.TabIndex = 0;
            this.transButton.Text = "전송";
            this.transButton.UseVisualStyleBackColor = true;
            this.transButton.Click += new System.EventHandler(this.transButton_Click);
            // 
            // outputBox
            // 
            this.outputBox.Location = new System.Drawing.Point(28, 24);
            this.outputBox.Name = "outputBox";
            this.outputBox.Size = new System.Drawing.Size(754, 358);
            this.outputBox.TabIndex = 2;
            this.outputBox.Text = "";
            this.outputBox.TextChanged += new System.EventHandler(this.outputBox_TextChanged);
            // 
            // inputBox
            // 
            this.inputBox.Location = new System.Drawing.Point(28, 397);
            this.inputBox.Name = "inputBox";
            this.inputBox.Size = new System.Drawing.Size(592, 39);
            this.inputBox.TabIndex = 3;
            this.inputBox.Text = "";
            this.inputBox.TextChanged += new System.EventHandler(this.inputBox_TextChanged);
            // 
            // ChatForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.inputBox);
            this.Controls.Add(this.outputBox);
            this.Controls.Add(this.transButton);
            this.Name = "ChatForm";
            this.Text = "ChatForm";
            this.Load += new System.EventHandler(this.ChatForm_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button transButton;
        private System.Windows.Forms.FontDialog fontDialog1;
        private System.Windows.Forms.RichTextBox outputBox;
        private System.Windows.Forms.RichTextBox inputBox;
    }
}