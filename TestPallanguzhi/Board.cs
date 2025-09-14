public class Board
{
	public int[] slots = new int[14];
	public void Initialize()
	{
		for(int i = 0; i < slots.Length; i++)
			slots[i] = 5;
	}

	public int Move(int index)
	{
		//Console.WriteLine("Given index is "+index);
		//Take all the coins inside the given and clear that slot.
		int coins = slots[index];		
		slots[index] = 0;
		//Start distributing from the next slot
		index = (index+1) % slots.Length;
		//Console.WriteLine("Starting distribution from " + index);

		//Distribute clockwise till the coins are exhausted
		while(coins > 0)
		{
			//Console.Write("increasing at " + index+ " before add " + slots[index]);
			slots[index] = slots[index] + 1;
			//Console.WriteLine(" hence coins present are " + slots[index]);
			index = (index+1) % slots.Length;
			coins--;
			if(coins == 0 && slots[index] != 0)
			{
				//Console.WriteLine("coins finished for distribution, Hence choosing coins in " + index + " coins are " + slots[index]);
				coins = slots[index];
				slots[index] = 0;
				index = (index+1) % slots.Length;
			}
		}
		index = (index+1) % slots.Length;
		coins = slots[index];
		slots[index] = 0;
		//Console.WriteLine("Score returned from index " + index);
		return coins;
	}

	public void Display()
	{
		for(int i = 13; i>=7; i--)
			Console.Write(slots[i]+" ");
		Console.WriteLine();

		for(int i = 0; i < 7; i++)
			Console.Write(slots[i]+" ");
		Console.WriteLine();
	}
}
