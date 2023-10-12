using System;

namespace BasicCalculator
{
 class BC
 {
	/* Returns true if c is a valid operator */
	static bool IsOperator (char c)
	{
	 switch (c)
	 {
		case '+':
		case '-':
		case '/':
		case '*':
		return true;
	 }
	 return false;
	}

	/* Returns true if the char is a space char */
	static bool IsSpace (char c)
	{
	 switch (c)
	 {
		case ' ': return true;
		case '\t': return true;
		case '\n': return true;
		case '\r': return true;
		case '\a': return true;
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

	static int Calculate (int n1, char op, int n2)
	{
	 switch (op)
	 {
		case '+':
		 return (n1 + n2);
		case '-':
		 return (n1 - n2);
		case '*':
		 return (n1 * n2);
		case '/':
		 return (n1 / n2);
		case '%':
		 return (n1 % n2);
	 }

	 return 0;
	}

	static void ReadLine (string[] argv)
	{
	 /* Assumes a int op int op pattern with the arguments
	  * To visualise this think 32 + 32 = 64 */
	 int argc = GetArgcString(argv);
	 int num1 = 0, num2 = 0, result = 0;
	 char op;

	 for (int i = 0; i < argc; i++)
	 {
		if (!int.TryParse(argv[i], out num1))
		{
		 Console.WriteLine("Couldn't convert " + argv[i] + " to an interger");
		 return;
		}

		op = argv[++i][0];

		if (!int.TryParse(argv[++i], out num2))
		{
		 Console.WriteLine("Couldn't convert " + argv[i] + " to an interger");
		 return;
		}

		result = Calculate(num1, op, num2);
		Console.WriteLine(result);
	 }
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

	 if (argc < 2)
	 {
		Console.WriteLine("Need at least 3 arguments to calculate");
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