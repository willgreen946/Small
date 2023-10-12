using System;

namespace BasicCalculator
{
 class BC
 {
	/* Stores the result of statement */
	static double result = 0;

	/* Returns true if the char is a space char */
	static bool IsSpace (char c)
	{
	 switch (c)
	 {
		case ' ': return true;
		case '\t': return true;
		case '\n': return true;
	 }

	 return false;
	}

	/* Returns the number of elements in a string array */
	static int GetArgcString (string[] argv)
	{
	 int argc;
	 for (argc = 0; argv[argc] != null; ++argc);
	 return argc;
	}

	/* Returns n1 plus n2 */
	static double Add (double n1, double n2)
	{
	 return (n1 + n2);
	}

	/* Returns n1 subtracted by n2 */
	static double Sub (double n1, double n2)
	{
	 return (n1 - n2);
	}

	/* Returns n1 multiplyed by n2 */
	static double Mul (double n1, double n2)
	{
	 return (n1 * n2);
	}

	/* Returns n1 divided by n2 */
	static double Div (double n1, double n2)
	{
	 return (n1 == 0 || n2 == 0) ? 0 : (n1 / n2);
	}

	/* Returns remainder of n1 divided by n2 */
	static double Mod (double n1, double n2)
	{
	 return (n1 == 0 || n2 == 0) ? 0 : (n1 % n2);
	}

	/* Returns result of n1 to the power of n2 */
	static double Pow (double n1, double n2)
	{
	 return Math.Pow(n1, n2);
	}

	static double Calculate (double n1, char op, double n2)
	{
	 switch (op)
	 {
		case '+':
		 return Add(n1, n2);
		case '-':
		 return Sub(n1, n2);
		case '*':
		 return Mul(n1, n2);
		case '/':
		 return Div(n1, n2);
		case '%':
		 return Mod(n1, n2);
		case '^':
		 return Pow(n1, n2);
		default:
		 Console.WriteLine("No such operator " + op);
		 return 0;
	 }
	}

	/* Reads from a array of strings (a line) */
	static void ReadLine (string[] argv)
	{
	 /* Assumes a this pattern 3 + 3 */
	 int argc = GetArgcString(argv);
	 double num1 = 0, num2 = 0;
	 char op;

	 for (int i = 0; i < argc; i++)
	 {
		if (argv[i] == "res")
		 num1 = result;

		else if (!double.TryParse(argv[i], out num1))
		{
		 Console.WriteLine("Couldn't convert " + argv[i] + " to an interger");
		 return;
		}

		op = argv[++i][0];

		if (argv[++i] == "res")
		 num2 = result;

		else if (!double.TryParse(argv[i], out num2))
		{
		 Console.WriteLine("Couldn't convert " + argv[i] + " to an interger");
		 return;
		}

		result = Calculate(num1, op, num2);
	 }

	 Console.WriteLine(result);
	}

	static void IsCommand (string str)
	{
	 if (str == "exit" || str == "quit")
		Environment.Exit(0);
	}

	/* This splits up a string by whitespace */
	static void ParseString (string calcString)
	{
	 int argc = 0;
	 string[] tokens = new string[255];

	 /* Place all the chars into a string array */
	 for (int i = 0; i < calcString.Length; ++i)
	 {
		/* Ignore whitespace */
		if (!IsSpace(calcString[i]))
		 tokens[argc] += calcString[i];
		else ++argc;
	 }

	 IsCommand(tokens[0]);

	 if (argc != 2)
	 {
		Console.WriteLine("Need 3 arguments to calculate");
		return;
	 }

	 ReadLine(tokens);
	}

	static void Main (string[] args)
	{
	 for (;;)
	 {
		string calcStr;

		calcStr = Console.ReadLine();
		ParseString(calcStr);
	 }
	}
 }
}