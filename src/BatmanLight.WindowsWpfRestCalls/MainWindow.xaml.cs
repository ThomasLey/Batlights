using System;
using System.Windows;
using System.Windows.Input;

namespace BatmanLightsControl
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public ICommand BatmanRedCommand { get; }
        public ICommand BatmanGreenCommand { get; }
        public ICommand BatmanBlueCommand { get; }
        public ICommand BatmanYellowCommand { get; }
        public ICommand BatmanCyanCommand { get; }
        public ICommand BatmanMagentaCommand { get; }
        public ICommand BatmanBlackCommand { get; }
        public ICommand BatmanWhiteCommand { get; }

        public ICommand BatmanSingleCommand { get; }
        public ICommand BatmanInvertCommand { get; }
        public ICommand BatmanAllCommand { get; }
        public ICommand BatmanKittCommand { get; }

        public ICommand BatmanTurtleCommand { get; }
        public ICommand BatmanVeryslowCommand { get; }
        public ICommand BatmanSlowCommand { get; }
        public ICommand BatmanMediumCommand { get; }
        public ICommand BatmanFastCommand { get; }
        public ICommand BatmanVeryfastCommand { get; }
        public ICommand BatmanMaxCommand { get; }

        public MainWindow()
        {
            InitializeComponent();

            BatmanRedCommand = new HttpCallCommand(SetTitle) { Uri = new Uri("http://batman/colorCode/red") };
            BatmanGreenCommand = new HttpCallCommand(SetTitle) { Uri = new Uri("http://batman/colorCode/green") };
            BatmanBlueCommand = new HttpCallCommand(SetTitle) { Uri = new Uri("http://batman/colorCode/blue") };
            BatmanYellowCommand = new HttpCallCommand(SetTitle) { Uri = new Uri("http://batman/colorCode/yellow") };
            BatmanCyanCommand = new HttpCallCommand(SetTitle) { Uri = new Uri("http://batman/colorCode/cyan") };
            BatmanMagentaCommand = new HttpCallCommand(SetTitle) { Uri = new Uri("http://batman/colorCode/magenta") };
            BatmanBlackCommand = new HttpCallCommand(SetTitle) { Uri = new Uri("http://batman/colorCode/black") };
            BatmanWhiteCommand = new HttpCallCommand(SetTitle) { Uri = new Uri("http://batman/colorCode/white") };

            BatmanSingleCommand = new HttpCallCommand(SetTitle) { Uri = new Uri("http://batman/animation/single") };
            BatmanInvertCommand = new HttpCallCommand(SetTitle) { Uri = new Uri("http://batman/animation/invert") };
            BatmanAllCommand = new HttpCallCommand(SetTitle) { Uri = new Uri("http://batman/animation/all") };
            BatmanKittCommand = new HttpCallCommand(SetTitle) { Uri = new Uri("http://batman/animation/kitt") };

            BatmanTurtleCommand = new HttpCallCommand(SetTitle) { Uri = new Uri("http://batman/speedCode/turtle") };
            BatmanVeryslowCommand = new HttpCallCommand(SetTitle) { Uri = new Uri("http://batman/speedCode/veryslow") };
            BatmanSlowCommand = new HttpCallCommand(SetTitle) { Uri = new Uri("http://batman/speedCode/slow") };
            BatmanMediumCommand = new HttpCallCommand(SetTitle) { Uri = new Uri("http://batman/speedCode/medium") };
            BatmanFastCommand = new HttpCallCommand(SetTitle) { Uri = new Uri("http://batman/speedCode/fast") };
            BatmanVeryfastCommand = new HttpCallCommand(SetTitle) { Uri = new Uri("http://batman/speedCode/veryfast") };
            BatmanMaxCommand = new HttpCallCommand(SetTitle) { Uri = new Uri("http://batman/speedCode/max") };

            DataContext = this;
        }

        public void SetTitle(string title)
        {
            Dispatcher.Invoke(() => { this.Title = $"Last Invoked State: {title}"; });
        }
    }
}
