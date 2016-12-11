// Podobny test s pracou so stringami
// ako v zadani projektu

class Main
{
	static void run()
	{
		StringClass.Citaj();
		ifj16.print("Uspesne si dokoncil test 4 !!!" + "\n");
	}

}

class StringClass
{
	static String vzor = "abcdef";
	static void Citaj()
	{
		ifj16.print("Napis retazec abc" + "\n");
		String nacitaj = ifj16.readString();
		int cmp = ifj16.compare(vzor, nacitaj);
		
		while (cmp != 0)
		{
			ifj16.print("Teraz napis abcdef" + "\n");
			nacitaj = ifj16.readString();
			cmp = ifj16.compare(vzor, nacitaj);
		}
	}
}
