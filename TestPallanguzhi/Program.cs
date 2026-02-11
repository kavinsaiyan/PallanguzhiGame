using System;
using System.Collections.Generic;
using System.Collections;
using System.Threading;
using System.Threading.Tasks;

public class Program
{
	public static void Main(string[] args)
	{
		//Console.WriteLine("Hello");
		Board board = new Board();
		board.Initialize();
		board.Display();

        int chosenIndex = 0;
        chosenIndex = 7;

		int scoreForChoosingZero = board.Move(chosenIndex);
		Console.WriteLine("Score for Choosing "+chosenIndex+" is " + scoreForChoosingZero);
		board.Display();

		//Cancellation token to pass to the update loop
		//CancellationTokenSource cts = new CancellationTokenSource();
		//Run the update loop that draws
		//Task.Run(() => UpdateLoop(cts));
		//take input in a while loop, break if want to quit otherwise clear and draw
		//while(true)
		//{
		//	string line = Console.ReadLine();
		//	if(line.Equals("q"))
		//	{
		//		cts.Cancel();
		//		break;
		//	}
		//}
		Console.WriteLine("Quiting..Done!");
	}

	public static async Task UpdateLoop(CancellationTokenSource cts)
	{
		while(!cts.IsCancellationRequested)
		{
			Console.WriteLine("Enter q to quit");
			await Task.Delay(1);
		}
	}
}
