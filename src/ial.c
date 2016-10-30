/** Interpretr jazyka IFJ16
* @file ial.c
* @author Kyzlink Jiří <xkyzli02@stud.fit.vutbr.cz>
* @author Kubiš Juraj <xkubis15@stud.fit.vutbr.cz>
* @author Korček Juraj <xkorce01@stud.fit.vutbr.cz>
* @author Kovařík Viktor <xkovar77@stud.fit.vutbr.cz>
* @author Kubica Jan <xkubic39@stud.fit.vutbr.cz>
*/

int find(char* s, char* search) {
// https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm
// http://www.ms.mff.cuni.cz/~kopecky/vyuka/dis/02/dis02_v3.html
}

char* sort(char* s) {      // razeni se snizujicim se prirustkem - Shell Sort
   int n = strlen(s) - 1;  // spocitani poctu prvku (bez koncoveho znaku "\0")
   int step = n / 2;       // jako prvni krok je bran pocet prvku lomen dvema (dle konvence)
   int i;                  // pomocna promenna k prochazeni stringu od prvniho k poslednimu prvku
   int j;                  // pomocna promenna k prochazeni paralelnich n-tic
   char h;                 // pomocna promenna pro prohozeni dvou hodnot

   while (step > 0) {            // dokud je krok vetsi jako nula
      for(i = 0; i < n; i++) {   // cykly pro generovani paralelnich n-tic
      //   printf("step=%d\n", step);
      //   printf("i=%d\n", i);
         j = i - step + 1;
      //   printf("j1=%d\n", j);       
         while (j >= 0 && s[j] > s[j + step]) { // prochazeni paralelnich n-tic
      //      printf("j2=%d\n", j);
            h = s[j];
            s[j] = s[j + step];
            s[j + step] = h;     // prohozeni dvojic na indexech rozdilnych o krok
            j = j - step;        // snizeni indexu o krok
         }
      }
      step = step / 2;           // redukce kroku dvema
   }

   return s;        
}