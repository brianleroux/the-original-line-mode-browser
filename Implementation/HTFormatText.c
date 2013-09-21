/*            LineMode Browser          Version 0.12             HTFormatText.c
**            ================                                      28-8-91
**
**  Author:
**      NP  Nicola Pellow  Tech.Student CERN 1990, 91
**
**      Copyright CERN 1990,91   See Copyright.html
**
**
** A module called from the WWW Line Mode Browser program WWW.c, concerned
** with storing the formatted text of a file in a linked list buffer, and
** outputting it onto the screen.  By storing the text in this buffer, it
** enables better paging and backwards scrolling.
*/

#ifdef SHORT_NAMES
#include "HTShort.h"
#define end_of_file		HTEOFile
#define end_of_formatted_buffer	HTEOFoBu
#endif

#include "HTUtils.h"
#include "tcp.h"
#include "HTFormatText.h"

extern int  HTScreenSize;
extern BOOL interactive;                /* User interaction ? */
extern BOOL end_of_file;	        /* End of the file? No more characters
                                           to be formatted */
extern BOOL end_of_formatted_buffer;    /* End of formatted text in buffer */
extern int  output_line_count;

#define LINE_NUM 0                      /* Trace check to show line numbers */

#define SCREEN_SIZE HTScreenSize        /* Number of lines to the screen */
#define SCREEN_WIDTH 80		        /* Number of characters across the
                                           width of the screen */

/*    Module Wide Types
**    -----------------
*/

    typedef  struct formatted_buffer {
 
             struct formatted_buffer  *next;

             struct formatted_buffer  *prev;

             char   line[SCREEN_WIDTH+1];

       } formatted_text;


/*  Module-Wide Variables
**  ---------------------
*/

PRIVATE formatted_text       *head,
                             *tail,           
                             *in_ptr,
                             *output_ptr;             


/*  Free Off Formatted Buffer Before Taking in a New File
**  =====================================================
**
**  Called when the formatted buffer is no longer neeed, i.e. when a new file 
**  is opened.
**
**  On entry,
**	No constraints. Buffers may or may not have been allocated
**  On exit,
**	Pointers are initialised, all buffers are freed.
*/

#ifdef __STDC__
PUBLIC void HTFormatBufferClose(void)
#else
PUBLIC void HTFormatBufferClose()
#endif

{
    while (head) {
	formatted_text * temp = head->next;
	free(head);
	head = temp;
    }
    head = tail = in_ptr = output_ptr = 0;       /* Initialise all pointers */
    end_of_formatted_buffer = NO;
    output_line_count = 1;

}


/*  Reads Each Formatted Line into a Double Linked List
**  ===================================================
**
**  Called everytime a new formatted line is produced.
**
**  On entry,
**       char * formatted_line  Formatted line passed from main program with
**                              margins included
**	
**  On exit,
**
*/

#ifdef __STDC__
PUBLIC void HTBufferFormattedLine(char * formatted_line)
#else
PUBLIC void HTBufferFormattedLine(formatted_line)
    char * formatted_line;
#endif

{
    in_ptr = (struct formatted_buffer *)malloc(sizeof(struct formatted_buffer));

    strcpy(in_ptr->line,formatted_line);

    if (head == 0){
        in_ptr->prev = 0;
        head = in_ptr;
        output_ptr = in_ptr;
    } else {
        tail->next = in_ptr;
        in_ptr->prev  = tail;
    }

    tail = in_ptr;

    in_ptr->next = 0;

}


/*  Outputs the Formatted Buffer
**  ============================
**
**  Outputs the text on the screen, checking for the end of the buffer and
**  formatted text.
*/

#ifdef __STDC__
PUBLIC void HTOutputLines(void)
#else
PUBLIC void HTOutputLines()
#endif

{
    if (output_ptr == tail){
        end_of_formatted_buffer = YES;
    } else {
        HTOutputBufferContent();
    }
}


/*  Prints on the Screen the Contents of the Buffer
**  ===============================================
**
**  Checks the layout of the text, looking at the vertical spacing, preventing  **  titles appearing on one page and the corresponding text on the next.
*/

#ifdef _STDC_
PUBLIC void HTOutputBufferContent(void)
#else
PUBLIC void HTOutputBufferContent()
#endif

{
/* If interactive - i.e. prompts for the user etc */

    if (interactive){

/* Looks at the first line of a page to check that it does not start with
** several blank lines.
*/
        if (output_line_count == 1){
        
            HTBackUpBufferPointer(); 

            if ((output_ptr != tail)&&
               (output_ptr->line[0] == '\0')){

                if (LINE_NUM){
                   printf("%d \n",output_line_count); /* The space is needed or
                                                         the line is skipped */
                } else {
                    printf(" \n");
                }

                output_line_count++;

                while ((output_ptr != tail)&&
                       (output_ptr->line[0] == '\0')){

                    output_ptr = output_ptr->next;
                }
            }
        }   
/* Prints out the main body of the page */

        while (output_line_count<=(SCREEN_SIZE - 4)&&(output_ptr!=tail)){

            if (LINE_NUM){
                printf("%d%s\n",output_line_count,output_ptr->line);
            } else {
                printf("%s\n",output_ptr->line);
            }
            output_ptr = output_ptr->next;
            output_line_count++;
        }

/* Checks the last four lines of a page to prevent splitting of small
** paragraphs, titles etc.
*/

        while ((output_line_count<=(SCREEN_SIZE - 1))&&
               (output_line_count>(SCREEN_SIZE - 4))&&
               (output_ptr!=tail)){

            if (output_ptr->line[0] == '\0'){
                while (output_line_count<=SCREEN_SIZE - 1){
                    if (LINE_NUM){
                        printf("%d\n",output_line_count);
                    } else {
                       printf("\n");
                    }
                    output_line_count++;
                }
                while (output_ptr->line[0] != '\0'){
                    output_ptr = output_ptr->next;
                }
                break;
            } else {
                if (LINE_NUM){
                    printf("%d%s\n",output_line_count,output_ptr->line);
                } else {
                    printf("%s\n",output_ptr->line);
                }
                output_ptr = output_ptr->next;
                output_line_count++;
            }
        }
    } else {                                 /* If non-interactive */

        if (output_ptr!=tail){
            printf("%s\n",output_ptr->line);
            output_ptr = output_ptr->next;
            output_line_count++;
        }
    }

    if (output_ptr == tail) end_of_formatted_buffer == YES;

}

/*  At the End of the Document Inserts Blank Lines to Tidy Screen
**  =============================================================
*/

#ifdef __STDC__
PUBLIC void HTEndofDocument(void)
#else
PUBLIC void HTEndofDocument()
#endif

{
/* If end of file when interactive. */

    if (LINE_NUM){
        printf("%d%s\n",output_line_count,tail->line);
    } else {
        printf("%s\n",tail->line);
    }
    output_line_count++;

    if (interactive){

        if (output_line_count<=(SCREEN_SIZE/2)){
            if (LINE_NUM){
                printf("%d\n",output_line_count);
                output_line_count++;
                printf("%d\n",output_line_count);
                output_line_count;
            } else {
                printf("\n");
                output_line_count++;
                printf("\n");
                output_line_count++;
            }
        } else {
            while (output_line_count <= (SCREEN_SIZE -1)){
                if (LINE_NUM){
                    printf("%d\n",output_line_count);
                } else {
                    printf("\n");
                }
                output_line_count++;
           }
           end_of_formatted_buffer = YES;
        }

/* If end of file when non-interactive */

    } else {
        exit(0);
    } 
}

/*  Moves the pointer to the current line in the buffer, back one
**  =============================================================
**  
**  Reproduces the last line of a page (if there is one), at the top of the 
**  next page, by moving the current line pointer in the buffer, back by one
**  line.
*/

#ifdef _STDC_
PRIVATE void HTBackUpBufferPointer(void)
#else
PRIVATE void HTBackUpBufferPointer()
#endif

{
    BOOL  line_of_spaces;
    int  character = 0;

    if (output_ptr->line[0]!='\0'){               /* Checks if line is empty */

        line_of_spaces == YES;                    /* Checks if line consists of 
                                                     entirely blank spaces */
        while (output_ptr->line[character]!='\0'){
            if (output_ptr->line[character]!=' '){
                line_of_spaces = NO;
                break;
            } else {
                character++;
            }
        }
        if ((output_ptr->prev)&&(line_of_spaces == NO)){

            if (output_ptr->prev->line[0]!='\0'){
       
                output_ptr = output_ptr->prev;

            }
        }
    } 
}


/*  Returns to the top page of the present node
**  ===========================================
**
**  Moves the output_ptr to the top of formatted buffer, so that the present
**  node can be read again.
*/

#ifdef __STDC__
PUBLIC void HTTopofNode(void)
#else
PUBLIC void HTTopofNode()
#endif

{
    output_ptr = head;

    output_line_count = 1;

    end_of_formatted_buffer =NO;

    HTOutputLines();
}


 
