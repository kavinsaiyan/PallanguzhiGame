using System;

public class Program
{
	public static void Main(string[] args)
	{
		Console.WriteLine("Hello");
		Board board = new Board();
		board.Initialize();
		board.Display();

		int scoreForChoosingZero = board.Move(0);
		//Console.WriteLine("Score for Choosing zero is " + scoreForChoosingZero);
		board.Display();


		
	}
}
