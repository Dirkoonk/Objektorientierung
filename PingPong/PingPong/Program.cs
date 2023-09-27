using System;
using System.Drawing;
using System.Windows.Forms;

public class PongGame : Form
{
    private const float GameWidth = 1600.0f; // Vollbildbreite
    private const float GameHeight = 900.0f; // Vollbildhöhe
    private const float PaddleWidth = 30.0f; // Breite des Paddles
    private const float PaddleHeight = 150.0f; // Höhe des Paddles
    private const float BallSize = 20.0f; // Größe des Balls
    private const float PaddleSpeed = 15.0f; // Geschwindigkeit der Paddles
    private const float BallSpeed = 8.0f; // Geschwindigkeit des Balls
    private const int MaxScore = 5; // Maximale Punktezahl zum Gewinnen
    private const int ComputerAIDelay = 10; // Verzögerung der Computer-KI
    private const float BorderSize = 5.0f; // Größe des schwarzen Rahmens um Paddles und Ball

    private float playerPaddleY;
    private float computerPaddleY;
    private float ballX;
    private float ballY;
    private float ballXSpeed;
    private float ballYSpeed;
    private int playerScore;
    private int computerScore;
    private Random random;

    private Timer gameTimer;
    private bool gameOver;
    private string gameResultText;
    private Font gameResultFont;
    private Font newGameFont;
    private Font ScoreFont;
    private bool newGame;
    private bool showGameOverText;
    private bool isMovingUp;
    private bool isMovingDown;

    public PongGame()
    {
        Text = "Pong Game";
        WindowState = FormWindowState.Maximized; // Starten Sie das Spiel im Vollbildmodus
        FormBorderStyle = FormBorderStyle.None; // Verbergen Sie den Formularrahmen
        Cursor.Hide(); // Verstecken Sie den Mauszeiger
        TopMost = true; // Setzen Sie das Spiel in den Vordergrund
        KeyDown += OnKeyDown; // Abonnieren des KeyDown-Ereignisses
        DoubleBuffered = true;

        playerPaddleY = GameHeight / 2 - PaddleHeight / 2;
        computerPaddleY = GameHeight / 2 - PaddleHeight / 2;
        ballX = GameWidth / 2 - BallSize / 2;
        ballY = GameHeight / 2 - BallSize / 2;
        ballXSpeed = BallSpeed;
        ballYSpeed = BallSpeed;
        playerScore = 0;
        computerScore = 0;
        random = new Random();

        gameTimer = new Timer();
        gameTimer.Interval = (int)(1000.0f / 120.0f);
        gameTimer.Tick += Update;
        gameTimer.Start();

        KeyPreview = true;

        gameResultText = string.Empty;
        gameResultFont = new Font(FontFamily.GenericSansSerif, 75.0f, FontStyle.Bold);
        newGameFont = new Font(FontFamily.GenericSansSerif, 35.0f, FontStyle.Bold);
        ScoreFont = new Font(FontFamily.GenericSansSerif, 20.0f, FontStyle.Bold);

        newGame = true;

        showGameOverText = true;
    }

    private void Update(object sender, EventArgs e)
    {
        if (gameOver)
        {
            showGameOverText = true;
        }
        else if (newGame)
        {
            showGameOverText = true;
        }

        Invalidate();

        if (gameOver || newGame)
        {
            return;
        }

        MoveBall();

        if (isMovingUp)
        {
            playerPaddleY -= PaddleSpeed;
        }
        else if (isMovingDown)
        {
            playerPaddleY += PaddleSpeed;
        }

        // Begrenzen Sie die Paddle-Position auf den Spielfeldbereich
        playerPaddleY = Math.Max(0.0f, Math.Min(GameHeight - PaddleHeight, playerPaddleY));

        MoveComputerPaddle();

        CheckForScoring();
    }

    private void StartNewGame()
    {
        playerScore = 0;
        computerScore = 0;
        ballX = GameWidth / 2 - BallSize / 2;
        ballY = GameHeight / 2 - BallSize / 2;
        ballXSpeed = BallSpeed;
        ballYSpeed = BallSpeed * (random.Next(2) == 0 ? 1 : -1);
        gameOver = false;
        newGame = false;
    }

    private void MoveBall()
    {
        ballX += ballXSpeed;
        ballY += ballYSpeed;

        if (ballY <= 0 || ballY >= GameHeight - BallSize)
        {
            ballYSpeed = -ballYSpeed;
        }

        if (ballX <= PaddleWidth && ballY >= playerPaddleY && ballY <= playerPaddleY + PaddleHeight)
        {
            ballXSpeed = -ballXSpeed*1.1f;
        }

        if (ballX >= GameWidth - PaddleWidth - BallSize && ballY >= computerPaddleY && ballY <= computerPaddleY + PaddleHeight)
        {
            ballXSpeed = -ballXSpeed*1.1f;
        }

        if (ballX <= 0 || ballX >= GameWidth)
        {
            if (ballX <= 0)
            {
                computerScore++;
            }
            else
            {
                playerScore++;
            }

            if (playerScore >= MaxScore || computerScore >= MaxScore)
            {
                gameOver = true;
                if (playerScore >= MaxScore)
                {
                    gameResultText = "You Won!";
                }
                else
                {
                    gameResultText = "You Lose!";
                }
            }
            else
            {
                ballX = GameWidth / 2 - BallSize / 2;
                ballY = GameHeight / 2 - BallSize / 2;
                ballXSpeed = BallSpeed;
                ballYSpeed = BallSpeed * (random.Next(2) == 0 ? 1 : -1);
            }
        }
    }

    private void MoveComputerPaddle()
    {
        if (ballY + BallSize / 2 < computerPaddleY + PaddleHeight / 2)
        {
            if (random.Next(ComputerAIDelay) == 0)
            {
                computerPaddleY -= PaddleSpeed;
            }
        }
        else
        {
            if (random.Next(ComputerAIDelay) == 0)
            {
                computerPaddleY += PaddleSpeed;
            }
        }

        computerPaddleY = Math.Max(0, Math.Min(GameHeight - PaddleHeight, computerPaddleY));
    }

    protected override void OnPaint(PaintEventArgs e)
    {
        base.OnPaint(e);

        e.Graphics.FillRectangle(Brushes.Gold, 0.0f, 0.0f, GameWidth, GameHeight);

        // Zeichnen Sie Paddles und Ball mit schwarzem Rahmen
        DrawWithBorder(e.Graphics, Brushes.Blue, Pens.Black, 10.0f, playerPaddleY, PaddleWidth, PaddleHeight, BorderSize);
        DrawWithBorder(e.Graphics, Brushes.Red, Pens.Black, GameWidth - 20.0f, computerPaddleY, PaddleWidth, PaddleHeight, BorderSize);
        DrawWithBorder(e.Graphics, Brushes.Green, Pens.Black, ballX, ballY, BallSize, BallSize, BorderSize);

        e.Graphics.DrawString($"Player: {playerScore}", ScoreFont, Brushes.OrangeRed, 10.0f, 10.0f);
        e.Graphics.DrawString($"Computer: {computerScore}", ScoreFont, Brushes.OrangeRed, GameWidth - 180.0f, 10.0f);

        if (showGameOverText)
        {
            e.Graphics.FillRectangle(Brushes.Black, 0.0f, 0.0f, GameWidth, GameHeight);
            e.Graphics.DrawString(gameResultText, gameResultFont, Brushes.White, GameWidth / 2 - 120.0f, GameHeight / 2 - 50.0f);
            e.Graphics.DrawString("Press \"Space\" for New Game", newGameFont, Brushes.White, GameWidth / 2 - 220.0f, GameHeight / 2 + 80.0f);
        }
    }

    protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
    {
        if (gameOver && keyData == Keys.Space)
        {
            StartNewGame();
            showGameOverText = false;
            Invalidate();
        }
        else if (newGame && keyData == Keys.Space)
        {
            newGame = false;
            StartNewGame();
            showGameOverText = false;
            Invalidate();
        }
        else if (!gameOver && !newGame)
        {
            // Wenn die "Oben"-Taste gedrückt wird, das Paddle nach oben bewegen
            if (keyData == Keys.Up)
            {
                isMovingUp = true;
                isMovingDown = false;
            }
            // Wenn die "Unten"-Taste gedrückt wird, das Paddle nach unten bewegen
            else if (keyData == Keys.Down)
            {
                isMovingDown = true;
                isMovingUp = false;
            }
        }

        return base.ProcessCmdKey(ref msg, keyData);
    }
    protected override void OnKeyUp(KeyEventArgs e)
    {
        base.OnKeyUp(e);

        if (!gameOver && !newGame)
        {
            // Wenn die "Oben" oder "Unten"-Taste losgelassen wird, das Paddle stoppen
            if (e.KeyCode == Keys.Up || e.KeyCode == Keys.Down)
            {
                isMovingUp = false;
                isMovingDown = false;
            }
        }
    }

    private void CheckForScoring()
    {
        if (ballX <= 0.0f || ballX >= GameWidth)
        {
            if (ballX <= 0.0f)
            {
                computerScore++;
            }
            else
            {
                playerScore++;
            }

            if (playerScore >= MaxScore || computerScore >= MaxScore)
            {
                gameOver = true;
                if (playerScore >= MaxScore)
                {
                    gameResultText = "You Won!";
                }
                else
                {
                    gameResultText = "You Lose!";
                }
            }
            else
            {
                ballX = GameWidth / 2 - BallSize / 2;
                ballY = GameHeight / 2 - BallSize / 2;
                ballXSpeed = BallSpeed;
                ballYSpeed = BallSpeed * (random.Next(2) == 0 ? 1 : -1);
            }
        }
    }

    private void DrawWithBorder(Graphics g, Brush fillBrush, Pen borderPen, float x, float y, float width, float height, float borderSize)
    {
        g.FillRectangle(borderPen.Brush, x - borderSize, y - borderSize, width + 2.0f * borderSize, height + 2.0f * borderSize);
        g.FillRectangle(fillBrush, x, y, width, height);
    }

    private void OnKeyDown(object sender, KeyEventArgs e)
    {
        if (e.KeyCode == Keys.Escape)
        {
            // Das Spiel beenden, wenn die Escape-Taste gedrückt wird
            Close();
        }
    }

    public static void Main()
    {
        Application.Run(new PongGame());
    }
}