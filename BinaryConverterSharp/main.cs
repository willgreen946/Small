using System;

/* Simple interger to binary converter in C# */

namespace BinaryConverter
{
 class BC
 {
	static void Main (string[] args)
	{
	 int num = 0;
	 string bin;

	 foreach (string arg in args)
	 {
		if (int.TryParse(arg, out num))
		{
		 /* Converts to base 2 */
		 bin = Convert.ToString(num, 2);
		 Console.WriteLine(bin);
		}

		else
		{
		 Console.WriteLine("Cannot convert " + arg + " to an interger!\nstopping ...");
		 break;
		}
	 }
	}
 }
}