class Main
{
	static void run()
	{
		int Rfib = math.Rfib(16);
		ifj16.print("Fib recursively: " + Rfib + "\n");
		int Ifib = math.Ifib(16);
		ifj16.print("Fib iteratively: " + Ifib + "\n");
		if(Ifib != Rfib)
		{
			ifj16.print("Error: " + Ifib + " != " + Rfib + "\n");
			ifj16.print("Correct result is:" + 987 + "\n");
			return;
		}
		else
		{
			ifj16.print("OK: " + Ifib + " == " + Rfib + "\n");
		}
		drawer.printLogo();
	}
}

class math
{
	static int Rfib(int n)
	{
		int first;
		int second;
		if(n <= 1)
		{
			return n;
		}
		else
		{
			first = n - 1;
			first = Rfib(first);
			second = n - 2;
			second = Rfib(second);
			return first + second;
		}
	}

	static int Ifib(int n)
	{
		int first = 0;
		int second = 1;
		int temp;
		int i = 1;
		while(i < n)
		{
			i = i + 1;
			temp = second;
			second = second + first;
			first = temp;
		}
		return second;
	}
}

class string
{

	static int generateText = 0;
	static int maxLen = 0;
	static String generatedText = "";
	static int len(String text)
	{
		return ifj16.length(text);
	}

	static String join(String text1, String text2)
	{
		text1 = text1 + text2;
		return text1;
	}

	static String generator(String pattern)
	{
		int length;
		if(generateText == 1)
		{
			length = system.len(generatedText);
			if(length >= maxLen)
			{
				generateText = 0;
			}
			else
			{
				generatedText = generatedText + pattern;
			}
			return generator(pattern);
		}
		else
		{
			return generatedText;
		}
	}

	static int find(String text, String pattern)
	{
		return ifj16.find(text, pattern);
	}

	static String substr(String text, int pos, int n)
	{
		return ifj16.substr(text, pos, n);
	}

	static int compare(String text, String text2)
	{
		return ifj16.compare(text, text2);
	}
}

class system
{
	static int len(String text)
	{
		return string.len(text);
	}

	static void printLine(String text)
	{
		text = string.join(text, "\n");
		ifj16.print(text);
	}

	static void print(String text)
	{
		ifj16.print(text);
	}

	static String generate(String text, int n)
	{
		string.generateText = 1;
		string.generatedText = "";
		string.maxLen = n;
		return string.generator(text);
	}
	static void testPattern(String text)
	{
		int position = string.find(text, "Yolo");
		position = position - 2;
		String blob = "SsYoloSwag41.Inc";
		int length = len(blob);
		String sub = string.substr(text, position, length);
		int result = string.compare(sub, blob);
		if(result != 0)
		{
			system.printLine("Error: found pattern incorrect!");
		}
		else
		{
			system.printLine("OK: found pattern Correct!");
		}
	}
}

class drawer
{
	static void printLogo()
	{
		String row = system.generate("*", 80);
		system.printLine(row);
		
		int i;

		i = 2;
		while(i > 0)
		{
			i = i - 1;
			row = system.generate("*",10);
			system.print(row);
			row = system.generate(" ",10);
			system.print(row);
			row = system.generate("*",10);
			system.print(row);
			row = system.generate(" ",20);
			system.print(row);
			row = system.generate("*",10);
			system.print(row);
			row = system.generate(" ",10);
			system.print(row);
			row = system.generate("*",10);
			system.printLine(row);
		}
		i = 2;
		while(i > 0)
		{
			i = i - 1;
			row = system.generate("*",10);
			system.print(row);
			row = system.generate(" ",10);
			system.print(row);
			row = system.generate("*",10);
			system.print(row);
			row = system.generate(" ",10);
			system.print(row);
			row = system.generate("*",20);
			system.print(row);
			row = system.generate(" ",10);
			system.print(row);
			row = system.generate("*",10);
			system.printLine(row);
		}
		i = 2;
		while(i > 0)
		{
			i = i - 1;
			row = system.generate("*",10);
			system.print(row);
			row = system.generate(" ",10);
			system.print(row);
			row = system.generate("*",10);
			system.print(row);
			row = system.generate(" ",15);
			system.print(row);
			row = system.generate("*",15);
			system.print(row);
			row = system.generate(" ",10);
			system.print(row);
			row = system.generate("*",10);
			system.printLine(row);
		}
		i = 2;
		while(i > 0)
		{
			i = i - 1;
			row = system.generate("*",10);
			system.print(row);
			row = system.generate(" ",10);
			system.print(row);
			row = system.generate("*",10);
			system.print(row);
			row = system.generate(" ",10);
			system.print(row);
			row = system.generate("*",20);
			system.print(row);
			row = system.generate(" ",10);
			system.print(row);
			row = system.generate("*",10);
			system.printLine(row);
		}
		row = system.generate("*",10);
		system.print(row);
		row = system.generate(" ",10);
		system.print(row);
		row = system.generate("*",10);
		system.print(row);
		row = system.generate(" ",10);
		system.print(row);
		row = system.generate("*",14);
		system.print(row);
		row = system.generate(" ",13);
		system.print(row);
		row = system.generate("*",13);
		system.printLine(row);

		row = system.generate("*",10);
		system.print(row);
		row = system.generate(" ",10);
		system.print(row);
		row = system.generate("*",10);
		system.print(row);
		row = system.generate(" ",10);
		system.print(row);
		row = system.generate("*",14);
		system.print(row);
		row = system.generate(" ",10);
		system.print(row);
		row = system.generate("*",16);
		system.printLine(row);
		row = system.generate("*",80);
		system.printLine(row);

		row = system.generate("*",31);
		row = string.join(row, " SsYoloSwag41.Inc ");
		String tmp = system.generate("*",31);
		row = string.join(row, tmp);
		system.printLine(row);
		system.testPattern(row);
	}
}
