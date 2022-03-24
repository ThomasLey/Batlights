using System;
using System.Net.Http;
using System.Windows.Input;

namespace BatmanLightsControl
{
    public class HttpCallCommand : ICommand
    {
        private readonly Action<string> _actionOnResult;

        public HttpCallCommand(Action<string>? actionOnResult = null)
        {
            _actionOnResult = actionOnResult ?? (s => { });
        }

        public Uri Uri { get; set; }

        public bool CanExecute(object? parameter)
        {
            return true;
        }

        public async void Execute(object? parameter)
        {
            var client = new HttpClient();
            var result = await client.GetStringAsync(Uri);
            Console.WriteLine(result);
            _actionOnResult(result);
        }

        public event EventHandler? CanExecuteChanged;
    }
}
