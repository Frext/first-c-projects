#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <conio.h>
#include <windows.h>
#include <unistd.h>

#define THE_FILE "The Note Names.txt"
#define TEMPORARY_FILE "Temp File.txt"
#define MAX_NOTE_TITLE_LENGTH 40
#define NO_NOTES_FOUND_RETURNING_STRING ""

typedef struct noteInfoStruct
{
  char *title;
  char *content;

} CreateNoteInfoStruct;

void Create_THE_FILE_IfItDoesntExist();

void WaitForSeconds_AndCleanScreen(float seconds);

void ObtainStringUntilNewline(char *string);

void IncreaseOrDecrease_decisionNum_AccordingTo_KbHit(short int *kbHit_return_value, unsigned short int *decisionNum, unsigned short int decisionNum_maxLimit);
int InterpretKbHit(); // kb = keyboard

int CheckIfFileHasContent(FILE *fptr);

void CopyAFileToAnother(FILE *destFile, FILE *sourceFile);

int ObtainDecision_AskToGoBackToMainMenu();
void DisplayDecisionScreen_AskToGoBackToMainMenu(unsigned short int *decisionNum);

//-----------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------//

// main menu
int ObtainDecision_ChooseMainOperation();
void DisplayDecisionScreen_ChooseMainOperation(unsigned short int *decisionNum);

// new note
char *ObtainTitle();
int CheckSameTitleOccurrences(char *title);
char *ObtainContent();
void CreateNewNoteFile(char *title, char *content);
void AppendNewTitleTo_THE_FILE(char *title);

// choose a note
int CountNoteTitlesOf_THE_FILE(FILE *THE_FILE_ptr);
char *ObtainDecision_ChooseANote(); // make changes in this function when you change MAX_NOTE_TITLE_LENGTH
void DisplayDecisionScreen_ChooseANote(unsigned short int *decisionNum, FILE *THE_FILE_ptr, char *TitleString);
int ObtainDecision_ChooseWhatToEdit();
void DisplayDecisionScreen_ChooseWhatToEdit(unsigned short int *decisionNum);

// edit title       ***
char *Obtain_NewTitle(char *oldTitle, char *newTitle);
void Rename_NoteTitle(char *oldTitle, char *newTitle);  // call needed functions
void Rename_NoteFilename(char *oldTitle, char *newTitle);
void Prepare_THE_FILE_ToBeEdited(char *title);
void Rewrite_NoteTitles_ExceptTheTitle(char *theTitle,FILE *THE_FILE_ptr,FILE *TEMPORARY_FILE_ptr);

// edit content
char *Obtain_NewContent(char *title);
void Print_OldContent(FILE *fptr);
void CountCharactersOF_NoteContent(unsigned short int *strLength, FILE *fptr);
void Change_NoteContent(char *title, char *newContent); 

// delete a note
void Delete_Note(char *title);
void Remove_NoteFile(char *title);

// browse notes
void Print_NoteTitleAndContent(char *title);


int main()
{
  Create_THE_FILE_IfItDoesntExist();

  CreateNoteInfoStruct Note;

mainMenuGOTOPoint: // be careful there is a GOTO label here
 
  switch (ObtainDecision_ChooseMainOperation())
  {
  case 0: /* NEW */

    printf("New Note\n--------\n\n");
    Note.title = ObtainTitle();
    printf("-------------------------------------------------------------------------\n");
    Note.content = ObtainContent();

    CreateNewNoteFile(Note.title, Note.content);
    AppendNewTitleTo_THE_FILE(Note.title);

    printf("\nSuccessfully created!\n");
    WaitForSeconds_AndCleanScreen(1);

    free(Note.title);
    free(Note.content);

    goto mainMenuGOTOPoint;

    break;

  case 1: /* EDIT */

    printf("Edit Note\n---------\n\n");
    Note.title = ObtainDecision_ChooseANote();

    if (strcmp(Note.title, NO_NOTES_FOUND_RETURNING_STRING) == 0)
    {
      goto mainMenuGOTOPoint;
    }

    char *newTitle, *newContent;

    switch (ObtainDecision_ChooseWhatToEdit())
    {
    case 0: // title
      newTitle = Obtain_NewTitle(Note.title, newTitle);
      Rename_NoteTitle(Note.title, newTitle);

      break;

    case 1: // content
      newContent = Obtain_NewContent(Note.title);
      Change_NoteContent(Note.title, newContent);
      break;

    case 2:
      goto mainMenuGOTOPoint;
    }

    printf("\nSuccessfully Edited!\n");
    WaitForSeconds_AndCleanScreen(1);

    free(Note.title);
    free(newTitle);
    free(newContent);

    goto mainMenuGOTOPoint;

    break;

  case 2: /* DELETE */

    printf("Delete Note\n-----------\n\n");
    Note.title = ObtainDecision_ChooseANote();

    if (strcmp(Note.title, NO_NOTES_FOUND_RETURNING_STRING) == 0)
    {
      goto mainMenuGOTOPoint;
    }

    Delete_Note(Note.title);

    printf("\nSuccessfully Deleted!\n");
    WaitForSeconds_AndCleanScreen(1);

    free(Note.title);

    goto mainMenuGOTOPoint;

    break;

  case 3: /* BROWSE */

    do
    {
      Note.title = ObtainDecision_ChooseANote();

      if (strcmp(Note.title, NO_NOTES_FOUND_RETURNING_STRING) == 0)
      {
        goto mainMenuGOTOPoint;
      }

      Print_NoteTitleAndContent(Note.title);

      free(Note.title);

    } while (ObtainDecision_AskToGoBackToMainMenu() == 0);

    goto mainMenuGOTOPoint;

    break;

  default: /* EXIT */
    system("CLS");
    remove(TEMPORARY_FILE);
    return 0;
  }
}

void Create_THE_FILE_IfItDoesntExist()
{
  if (access(THE_FILE, F_OK) != 0) // 0 = file exist , -1 = file doesn't exist
  {
    FILE *THE_FILE_ptr = fopen(THE_FILE, "a");

    if (THE_FILE_ptr != NULL)
    {
      fclose(THE_FILE_ptr);
    }
    else
    {
      puts("-> There was an issue while creating The Note Names.txt , error code : 0");
      exit(EXIT_FAILURE);
    }
  }
}

void WaitForSeconds_AndCleanScreen(float seconds)
{
  seconds *= 1000;
  Sleep(seconds);
  system("CLS");
}

void ObtainStringUntilNewline(char *string)
{
  unsigned short int i;
  for (i = 0; i < strlen(string); i++)
  {
    if ((string[i] == '\n'))
    {
      break;
    }
  }
  string[i] = '\0';
}



int InterpretKbHit()
{
  int charInput, up_andDownArrowKeys = 0;
  while (1)
  {
    if (kbhit() == 1)
    {
      charInput = getch();

      if (charInput == 0xE0) // changing up or down arrow key into 'w' or 's'
      {
        up_andDownArrowKeys = getch();

        switch (up_andDownArrowKeys)
        {
        case 72: // up arrow key
          charInput = 119;
          break;

        case 80: // down arrow key
          charInput = 115;
          break;
        }
      }

      switch (charInput)
      {
      case 119: // lowercase 'w'
      case 87:  // uppercase 'W'

        return 1;

      case 115: // lowercase 's'
      case 83:  // uppercase 'S'

        return -1;

      case 13: // enter key , carriage return

        return 0;

      default: // unvalid keys

        break;
      }
    }
  }
}

void IncreaseOrDecrease_decisionNum_AccordingTo_KbHit(short int *kbHit_return_value, unsigned short int *decisionNum, unsigned short int decisionNum_maxLimit)
{
  (*kbHit_return_value) = InterpretKbHit();

  if ((*kbHit_return_value) == 1) // W or up arrow key
  {
    if ((*decisionNum) != 0)
    {
      (*decisionNum)--;
    }
  }

  else if ((*kbHit_return_value) == -1) // S or down arrow key
  {
    if ((*decisionNum) != decisionNum_maxLimit)
    {
      (*decisionNum)++;
    }
  }

  // another thing for returning zero
}

int CheckIfFileHasContent(FILE *fptr)
{
  unsigned short int fileSize = 0;

  fseek(fptr, 0, SEEK_END);
  fileSize = ftell(fptr);
  rewind(fptr);

  if (fileSize > 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

void CopyAFileToAnother(FILE *destFile, FILE *sourceFile)
{
  rewind(sourceFile), rewind(destFile);

  int c = getc(sourceFile);
  while (c != EOF)
  {
    putc(c, destFile);
    c = getc(sourceFile);
  }

  rewind(destFile);
}

int ObtainDecision_AskToGoBackToMainMenu()
{
  int charInput;
  unsigned short int decisionNum = 0;
  short int kbHit_return_value;
  while (1)
  {
    DisplayDecisionScreen_AskToGoBackToMainMenu(&decisionNum);

    IncreaseOrDecrease_decisionNum_AccordingTo_KbHit(&kbHit_return_value, &decisionNum, 1);
    system("CLS");

    if (kbHit_return_value == 0)  // if enter is pressed
    {
      if (decisionNum == 1) 
      {
        return 0; // keep browsing notes
      }
      else
      {
        return 1; // go back to the main menu
      }
    }
  }
}

void DisplayDecisionScreen_AskToGoBackToMainMenu(unsigned short int *decisionNum)
{
  printf("\n / Do you want to go back to the Main Menu or keep browsing?\n\n");
  switch ((*decisionNum))
  {
  case 0:
    puts("> Go back to the Main Menu");
    puts("  Keep Browsing");
    break;

  case 1:
    puts("  Go back to the Main Menu");
    puts("> Keep Browsing");
    break;
  }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------------------------------//

int ObtainDecision_ChooseMainOperation()
{
  unsigned short int decisionNum = 0;
  short int kbHit_return_value;

  while (1)
  {
    DisplayDecisionScreen_ChooseMainOperation(&decisionNum);

    IncreaseOrDecrease_decisionNum_AccordingTo_KbHit(&kbHit_return_value, &decisionNum, 4);
    if (kbHit_return_value == 0) // Enter
    {
      system("CLS");
      return decisionNum;
    }
  }
}

void DisplayDecisionScreen_ChooseMainOperation(unsigned short int *decisionNum)
{
  system("CLS");

  printf("Main Menu\n---------\n\n");

  switch ((*decisionNum))
  {
  case 0:
    puts("> New Note");
    puts("  Edit Note");
    puts("  Delete Note");
    puts("  Browse Notes");
    puts("  Exit");
    break;

  case 1:
    puts("  New Note");
    puts("> Edit Note");
    puts("  Delete Note");
    puts("  Browse Notes");
    puts("  Exit ");
    break;

  case 2:
    puts("  New Note");
    puts("  Edit Note");
    puts("> Delete Note");
    puts("  Browse Notes");
    puts("  Exit");
    break;

  case 3:
    puts("  New Note");
    puts("  Edit Note");
    puts("  Delete Note");
    puts("> Browse Notes");
    puts("  Exit");
    break;

  case 4:
    puts("  New Note");
    puts("  Edit Note");
    puts("  Delete Note");
    puts("  Browse Notes");
    puts("> Exit");
    break;
  }

  fflush(stdout);
}

///////////////////////////////////////////////////////////

char *ObtainTitle()
{
  char *TitleString;
  char charInput; // to get char
  unsigned short int indexNum;

  while(1) 
  {
    printf("Please enter the title ( max %d characters ): ",MAX_NOTE_TITLE_LENGTH);
    
    TitleString = (char *)malloc(sizeof(char));
    TitleString[0] = 0;   // this is NULL

    indexNum = 0;

    while (((charInput = getchar()) != '\n'))
    {
      TitleString[indexNum] = charInput;
      indexNum++;

      TitleString = (char *)realloc(TitleString, sizeof(char));
    }
    TitleString[indexNum] = '\0';

    if ( TitleString[0] == 0 )
    {
      puts("\nA title cannot be zero-long. Please try again!");
      
      free(TitleString);

      WaitForSeconds_AndCleanScreen(1);
    }

    else if(strlen(TitleString) > MAX_NOTE_TITLE_LENGTH)
    {
      puts("\nThis title is too long. Please try again!");

      free(TitleString);

      WaitForSeconds_AndCleanScreen(1);
    }

    else if (CheckSameTitleOccurrences(TitleString) == 1)
    {
      printf("\nThere is already a note named \"%s\". Please enter a different title.", TitleString);

      free(TitleString);

      WaitForSeconds_AndCleanScreen(2);
    }

    else
    {
      break;
    }
  }
  return TitleString;
}

int CheckSameTitleOccurrences(char *title)
{
  if (access(THE_FILE, F_OK) == 0)
  {
    FILE *THE_FILE_ptr = fopen(THE_FILE, "r");

    char temp_TitleString[MAX_NOTE_TITLE_LENGTH + 1]; // with '\0'

    if (THE_FILE_ptr != NULL)
    {
      while (!feof(THE_FILE_ptr))
      {
        fgets(temp_TitleString, MAX_NOTE_TITLE_LENGTH + 1, THE_FILE_ptr);

        ObtainStringUntilNewline(temp_TitleString);

        if (strcmp(temp_TitleString, title) == 0)
        {
          return 1;
        }
      }
    }
    else
    {
      puts("\n-> There was an issue with The Note Names.txt , error code : 1");
      exit(EXIT_FAILURE);
    }

    fclose(THE_FILE_ptr);
  }
  return 0;
}

char *ObtainContent()
{
  printf("Please enter the content : ");

  char *ContentString = (char *)malloc(sizeof(char));
  char charInput; // to get char
  unsigned short int indexNum = 0;

  while (((charInput = getchar()) != '\n'))
  {
    ContentString[indexNum] = charInput;
    indexNum++;

    ContentString = (char *)realloc(ContentString, sizeof(char));
  }

  // you can enter zero-long content
  // and can enter a lot of characters

  ContentString[indexNum] = '\0';
  return ContentString;
}

void CreateNewNoteFile(char *title, char *content)
{
  strcat(title, ".txt");
  FILE *newFile_ptr = fopen(title, "w");
  strtok(title, ".txt");

  if (newFile_ptr != NULL)
  {
    fprintf(newFile_ptr, "%s\n", content);
    fflush(stdout);
    fclose(newFile_ptr);
  }
  else
  {
    puts("\n-> There was an issue with creating the new note file , error code : 2");
    exit(EXIT_FAILURE);
  }
}

void AppendNewTitleTo_THE_FILE(char *title)
{
  FILE *THE_FILE_ptr = fopen(THE_FILE, "a");

  if (THE_FILE_ptr != NULL)
  {
    fprintf(THE_FILE_ptr, "%s\n", title);
    fflush(stdout);
    fclose(THE_FILE_ptr);
  }
  else
  {
    puts("\n-> There was an issue with The Note Names.txt , error code : 3");
    exit(EXIT_FAILURE);
  }
}

///////////////////////////////////////////////////////////

int CountNoteTitlesOf_THE_FILE(FILE *THE_FILE_ptr)
{
  char c;
  unsigned short int counter = 0;

  rewind(THE_FILE_ptr);

  while (!feof(THE_FILE_ptr))
  {
    c = getc(THE_FILE_ptr);
    if ((c == '\n')) // increment the counter
    {
      counter++;
    }
  }

  rewind(THE_FILE_ptr);
  return counter;
}

char *ObtainDecision_ChooseANote()
{
  FILE *THE_FILE_ptr = fopen(THE_FILE, "r");

  if (THE_FILE_ptr != NULL)
  {
    if (CheckIfFileHasContent(THE_FILE_ptr) == 1)
    {
      unsigned short int noteTitles_Num = CountNoteTitlesOf_THE_FILE(THE_FILE_ptr);
      unsigned short int decisionNum = 0;
      short int kbHit_return_value;

      char *TitleString = (char *)malloc(sizeof(char) * (MAX_NOTE_TITLE_LENGTH + 5));
      // x characters + ".txt" + '\0'

      while (1)
      {
        DisplayDecisionScreen_ChooseANote(&decisionNum, THE_FILE_ptr, TitleString);

        IncreaseOrDecrease_decisionNum_AccordingTo_KbHit(&kbHit_return_value, &decisionNum, noteTitles_Num - 1);
        /*
         Why (noteTitles_Num - 1) ? Because if there is just one note, then it's decision value is going to be 0, right?
         But the max value will be 0 not 1.
        */
        if (kbHit_return_value == 0)
        {
          fclose(THE_FILE_ptr);
          return TitleString;
        }
      }
    }
    else
    {
      puts("There is no note to choose. Please create a note first.");
      WaitForSeconds_AndCleanScreen(1);
      puts("Directing to the Main Menu...");
      WaitForSeconds_AndCleanScreen(0.5);

      fflush(stdout);
      fclose(THE_FILE_ptr);

      return NO_NOTES_FOUND_RETURNING_STRING;
    }
  }
  else
  {
    puts("\n-> There was an issue with The Note Names.txt , error code : 4");
    exit(EXIT_FAILURE);
  }

  fclose(THE_FILE_ptr);
}

void DisplayDecisionScreen_ChooseANote(unsigned short int *decisionNum, FILE *THE_FILE_ptr, char *TitleString)
{
  FILE *noteFile_ptr;
  unsigned short int Counter_toPlaceThePointer = 0;

  char temp_TitleString[MAX_NOTE_TITLE_LENGTH + 5]; // with '\0' and ".txt"
  char temp_ContentString[22];

  system("CLS");

  printf("             Title                         |          Content\n             -----                                    -------\n\n");

  while (1)
  {
    fgets(temp_TitleString, MAX_NOTE_TITLE_LENGTH + 1, THE_FILE_ptr);
    ObtainStringUntilNewline(temp_TitleString);

    if(feof(THE_FILE_ptr))
    {
      break;
    }

    strcat(temp_TitleString, ".txt");
    noteFile_ptr = fopen(temp_TitleString, "r");
    strtok(temp_TitleString, ".txt");

    if (noteFile_ptr != NULL)
    {
      fgets(temp_ContentString, 22, noteFile_ptr); // we get 21 characters to place "..." if it has more chararacters than 20 
      ObtainStringUntilNewline(temp_ContentString);

      fclose(noteFile_ptr);

      if (Counter_toPlaceThePointer == (*decisionNum))
      {
        printf(">");
        strcpy(TitleString, temp_TitleString);
      }
      else
      {
        printf(" ");
      }

      Counter_toPlaceThePointer++;

      printf(" %-40s |  %-20s", temp_TitleString, temp_ContentString);

      if(strlen(temp_ContentString) == 21) // that's the reason why we get 21 characters up there
      {
        printf("...");
      }
      printf("\n");
    }
    else
    {
      puts("\n-> There was an issue with one of the note files , error code : 5");
      exit(EXIT_FAILURE);
    }
  }
  rewind(THE_FILE_ptr);
}

int ObtainDecision_ChooseWhatToEdit()
{
  unsigned short int decisionNum = 0;
  short int kbHit_return_value;

  while (1)
  {
    DisplayDecisionScreen_ChooseWhatToEdit(&decisionNum);

    IncreaseOrDecrease_decisionNum_AccordingTo_KbHit(&kbHit_return_value, &decisionNum, 2);

    if (kbHit_return_value == 0)
    {
      system("CLS");
      return decisionNum;
    }
  }
}

void DisplayDecisionScreen_ChooseWhatToEdit(unsigned short int *decisionNum)
{
  system("CLS");
  printf("What to edit?\n-------------\n\n");

  switch ((*decisionNum))
  {
  case 0:
    puts("> Title");
    puts("  Content");
    puts("  Go back to the Main Menu");
    break;
  case 1:
    puts("  Title");
    puts("> Content");
    puts("  Go back to the Main Menu");
    break;
  case 2:
    puts("  Title");
    puts("  Content");
    puts("> Go back to the Main Menu");
    break;
  }
  fflush(stdout);
}

///////////////////////////////////////////////////////////

char *Obtain_NewTitle(char *oldTitle, char *newTitle)
{
  printf("Old Title \"%s\"\n\n", oldTitle);
  printf("New Title\n---------\n\n");
  fflush(stdout);

  char *strPtr = ObtainTitle();
  return strPtr;
}

void Rename_NoteTitle(char *oldTitle, char *newTitle)
{
  Rename_NoteFilename(oldTitle, newTitle);
  Prepare_THE_FILE_ToBeEdited(oldTitle);
  AppendNewTitleTo_THE_FILE(newTitle);
}

void Rename_NoteFilename(char *oldTitle, char *newTitle)
{
  strcat(oldTitle, ".txt");
  strcat(newTitle, ".txt");

  rename(oldTitle, newTitle);

  strtok(oldTitle, ".txt");
  strtok(newTitle, ".txt");
}

void Prepare_THE_FILE_ToBeEdited(char *title)
{
  FILE *THE_FILE_ptr = fopen(THE_FILE, "r");
  if (THE_FILE_ptr != NULL)
  {
    FILE *TEMPORARY_FILE_ptr = fopen(TEMPORARY_FILE, "w+");

    if (TEMPORARY_FILE_ptr != NULL)
    {
      CopyAFileToAnother(TEMPORARY_FILE_ptr, THE_FILE_ptr);

      fclose(THE_FILE_ptr);
      THE_FILE_ptr = fopen(THE_FILE, "w");

      if (THE_FILE_ptr != NULL)
      {
        Rewrite_NoteTitles_ExceptTheTitle(title,THE_FILE_ptr,TEMPORARY_FILE_ptr);
        fclose(THE_FILE_ptr);
        fclose(TEMPORARY_FILE_ptr);
      }
      else
      {
        puts("\n-> There was an issue with The Note Names.txt , error code : 6");
        exit(EXIT_FAILURE);
      }
    }
    else
    {
      puts("\n-> There was an issue with Temp File.txt , error code : 7");
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    puts("\n-> There was an issue with The Note Names.txt , error code : 8");
    exit(EXIT_FAILURE);
  }
}

void Rewrite_NoteTitles_ExceptTheTitle(char *theTitle,FILE *THE_FILE_ptr,FILE *TEMPORARY_FILE_ptr)
{
  char tempString[MAX_NOTE_TITLE_LENGTH + 1];

  while (1)
  {
      fgets(tempString, MAX_NOTE_TITLE_LENGTH + 1, TEMPORARY_FILE_ptr);
      ObtainStringUntilNewline(tempString);

      if( feof(TEMPORARY_FILE_ptr) )
      {
        break;
      }

      else if ((strcmp(theTitle, tempString) != 0) )
      {
        fprintf(THE_FILE_ptr, "%s\n", tempString);
        fflush(stdout);
      }
  }
}

char *Obtain_NewContent(char *title)
{
  strcat(title, ".txt");
  FILE *fptr = fopen(title, "r");
  strtok(title, ".txt");

  if (fptr != NULL)
  {
    Print_OldContent(fptr);

    fclose(fptr);

    printf("\nNew Content\n-----------\n\n");
    fflush(stdout);

    char *strPtr = ObtainContent();
    return strPtr;
  }
  else
  {
    puts("\n-> There was an issue with the note file , error code : 9");
    exit(EXIT_FAILURE);
  }
}

void Print_OldContent(FILE *fptr)
{
  unsigned short int strLength = 0, i;
  char c;

  CountCharactersOF_NoteContent(&strLength, fptr);

  printf("Old Content\n");

  for (i = 0; i < strLength; i++)
  {
    printf("-");
  }
  printf("\n\n");

  c = getc(fptr);
  while (c != EOF)
  {
    if(c != '\n') 
    {
      printf("%c", c);
    }
    c = getc(fptr);
    
  }

  printf("\n\n");
  for (i = 0; i < strLength; i++)
  {
    printf("-");
  }

  printf("\n\n");

  fflush(stdout);
}

void CountCharactersOF_NoteContent(unsigned short int *strLength, FILE *fptr)
{
  fseek(fptr, 0, SEEK_END);
  (*strLength) = ftell(fptr);
  rewind(fptr);
}

void Change_NoteContent(char *title, char *newContent)
{
  strcat(title, ".txt");
  FILE *fptr = fopen(title, "w");
  strtok(title, ".txt");

  if (fptr != NULL)
  {
    fprintf(fptr, "%s", newContent);
    fflush(stdout);
    fclose(fptr);
  }
  else
  {
    puts("\n-> There was an issue with the note file , error code : 10");
    exit(EXIT_FAILURE);
  }
}

void Delete_Note(char *title)
{
  Remove_NoteFile(title);
  Prepare_THE_FILE_ToBeEdited(title);
}

void Remove_NoteFile(char *title)
{
  strcat(title, ".txt");
  remove(title);
  strtok(title, ".txt");
}

void Print_NoteTitleAndContent(char *title)
{
  strcat(title,".txt");
  FILE *fptr = fopen(title,"r");
  strtok(title,".txt");

  if(fptr!=NULL)
  {
    system("CLS");

    printf("%s\n",title);
    for(unsigned short int i = 0 ; i < strlen(title) ; i++)
    {
      printf("-");
    }
    printf("\n\n");

    fflush(stdout);

    char c = getc(fptr);
    while(!feof(fptr))
    {
      printf("%c",c);
      c = getc(fptr);
    }
    printf("\n\n\n");

    fflush(stdout);
    fclose(fptr);
  }
  else
  {
    puts("\n-> There was an issue with the note file , error code : 11");
    exit(EXIT_FAILURE);
  }
}
