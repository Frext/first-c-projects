#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char AllowedPasswordCharacters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789~`!@#$%^&*()_-+={[}]|\\:;\"'<,>.?/"; 
// I will refer this string as "the alphabet".


unsigned long long int PasswordCount = 0;

int main(void) // 10-Digit Password Cracker  
{
  // Assign the length of the alphabet to a variable, so I don't need to call strlen() every time I need the length.
  unsigned int lengthOfTheAlphabet = strlen(AllowedPasswordCharacters);

  // You can add or remove for loops by the digit count you want to print.
  for (unsigned int index0 = 0; index0 < lengthOfTheAlphabet; index0++)
  {
    char index0_char = AllowedPasswordCharacters[index0]; // Don't call AllowedPasswordCharacters[indexNUMBER] every time. Instead, use a character.

    for (unsigned int index1 = 0; index1 < lengthOfTheAlphabet; index1++)
    {
      char index1_char = AllowedPasswordCharacters[index1];

      for (unsigned int index2 = 0; index2 < lengthOfTheAlphabet; index2++)
      {
        char index2_char = AllowedPasswordCharacters[index2];

        for (unsigned int index3 = 0; index3 < lengthOfTheAlphabet; index3++)
        {
          char index3_char = AllowedPasswordCharacters[index3];

          for (unsigned int index4 = 0; index4 < lengthOfTheAlphabet; index4++)
          {
            char index4_char = AllowedPasswordCharacters[index4];

            for (unsigned int index5 = 0; index5 < lengthOfTheAlphabet; index5++)
            {
              char index5_char = AllowedPasswordCharacters[index5];

              for (unsigned int index6 = 0; index6 < lengthOfTheAlphabet; index6++)
              {
                char index6_char = AllowedPasswordCharacters[index6];

                for (unsigned int index7 = 0; index7 < lengthOfTheAlphabet; index7++)
                {
                  char index7_char = AllowedPasswordCharacters[index7];

                  for (unsigned int index8 = 0; index8 < lengthOfTheAlphabet; index8++)
                  {
                    char index8_char = AllowedPasswordCharacters[index8];

                    for (unsigned int index9 = 0; index9 < lengthOfTheAlphabet; index9++)
                    {
                      char index9_char = AllowedPasswordCharacters[index9];

                      // You can remove the spaces between the characters. 
                      printf("%c %c %c %c %c %c %c %c %c %c\n",index0_char,index1_char,index2_char,index3_char,index4_char,index5_char,index6_char,index7_char,index8_char,index9_char);
                      fflush(stdout);

                      PasswordCount++;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  printf("\n\n|%llu| passwords found.",PasswordCount);
}
