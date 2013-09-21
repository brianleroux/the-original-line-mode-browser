/*                                                           HTFormatText.c
**                                                              3-7-91
**
** A module to be called from the WWW Line Mode Browser program WWW.c, to store 
** the formatted text of a file. This is particulary good for improving the
** paging code so that it is more sensitive towards things like headings
** appearing at the bottom of pages. It also can be used for a scrolling 
** system aswell outputting the text into a file rather than straight onto the
** screen.
*/

#include "HTUtils.h"
#include "tcp.h"
#include <stdio.h>
#include "HTFormatText.h"

#ifdef SHORT_NAMES
#include "HTShort.h"
#endif


extern int  HTScreenSize;
extern BOOL interactive;                /* User interaction ? */
extern BOOL end_of_file;	        /* Is it the end of the file? */
extern BOOL bottom_of_formatted_buffer; /* End of formatted text in buffer */

#define LINE_NUM 0                      /* Trace check to show line numbers */
#define FUNCTION_CHECK 0
#define TAIL_CHECK 0

#define SCREEN_SIZE HTScreenSize      /* Number of lines to the screen */
#define SCREEN_WIDTH 80		      /* Number of characters across the width
                                         of the screen */

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

PRIVATE int  num_of_lines; 


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
    bottom_of_formatted_buffer = NO;
    num_of_lines = 1;

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
    if (FUNCTION_CHECK) printf("<HTBufferFormattedLine>");

    in_ptr = (struct formatted_buffer *)malloc(sizeof(struct formatted_buffer));

    strcpy(in_ptr->line,formatted_line);

    if (head == 0){
        head = in_ptr;
        output_ptr = in_ptr;
        in_ptr->prev = 0;
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
**  Outputs the formatted text from a double linked list buffer, one page at a
**  time. Checks to see if there is enough formatted text in the buffer to
**  produce a complete page. If not, calls Format_Text() to send to the buffer
**  more formatted characters.
*/

#ifdef __STDC__
PUBLIC void HTOutputLines(void)
#else
PUBLIC void HTOutputLines()
#endif

{
    if (FUNCTION_CHECK) printf("<HTOutputLines>");

    if (!((end_of_file)&&(output_ptr == tail))){

        while (num_of_lines <= (SCREEN_SIZE - 1)){

            if (output_ptr == tail){
                if (end_of_file){
                    break;
                } else {
                    Format_Lines();
                } 
            }
            HTOutputBufferContent();
        }
        num_of_lines = 1;               

        if ((end_of_file)&&(output_ptr == tail)){
            HTEndofDocument();
        }
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
    if (FUNCTION_CHECK) printf("<HTOutputBufferContent>"); 

/* If interactive - i.e. prompts for the user etc */

    if (interactive){

/* Looks at the first line of a page to check that it does not start with
** several blank lines.
*/
        if ((num_of_lines == 1)&&
            (output_ptr != tail)&&
            (output_ptr->line[0] == '\0')){

             if (LINE_NUM){
                printf("%d \n",num_of_lines);     /* The space is needed or the 
                                                     line is skipped !?@@*/
            } else {
                printf(" \n");
            } 
            num_of_lines++;

            while ((output_ptr != tail)&&
                   (output_ptr->line[0] == '\0')){

                output_ptr = output_ptr->next;
            }
        }    

/* Prints out the main body of the page */

	while (num_of_lines<=(SCREEN_SIZE - 4)&&(output_ptr!=tail)){

            if (LINE_NUM){
	        printf("%d%s\n",num_of_lines,output_ptr->line);
            } else {
                printf("%s\n",output_ptr->line);
            }
	    output_ptr = output_ptr->next;
	    num_of_lines++;
	}

/* Checks the last four lines of a page to prevent splitting of small 
** paragraphs, titles etc.
*/

	while ((num_of_lines<=(SCREEN_SIZE - 1))&&
               (num_of_lines>(SCREEN_SIZE - 4))&&
               (output_ptr!=tail)){  
    
            if (output_ptr->line[0] == '\0'){
                while (num_of_lines<=SCREEN_SIZE - 1){
                    if (LINE_NUM){
                        printf("%d\n",num_of_lines);
                    } else {
                       printf("\n");
                    }
                    num_of_lines++;
                }
                while (output_ptr->line[0] != '\0'){
                    output_ptr = output_ptr->next;
                }
                break;
            } else {
                if (LINE_NUM){
	            printf("%d%s\n",num_of_lines,output_ptr->line);        
                } else {
                    printf("%s\n",output_ptr->line);
                }
	        output_ptr = output_ptr->next;    
	        num_of_lines++;
	    } 
        }
    } else {                                 /* If non-interactive */

        if (output_ptr!=tail){
            printf("%s\n",output_ptr->line);
            output_ptr = output_ptr->next;
            num_of_lines++;
        }
    }
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
    if (FUNCTION_CHECK) printf("<HTEndofDocument>");

/* If end of file when interactive. */ 

    if (TAIL_CHECK) printf("<TAIL>");

    if (LINE_NUM){
        printf("%d%s\n",num_of_lines,tail->line);
    } else {
        printf("%s\n",tail->line);
    }
    num_of_lines++; 

    if (interactive){ 

	if (num_of_lines<=(SCREEN_SIZE/2)){
            if (LINE_NUM){
	        printf("%d\n",num_of_lines);
                num_of_lines++;
                printf("%d\n",num_of_lines);
                num_of_lines++;
            } else {
                printf("\n");
                num_of_lines++;
	        printf("\n");
                num_of_lines++;
            }
	} else {
	    while (num_of_lines <= (SCREEN_SIZE -1)){
                if (LINE_NUM){
	            printf("%d\n",num_of_lines);
                } else {
                    printf("\n");
                }
	        num_of_lines++;
           }   
           bottom_of_formatted_buffer = YES;
        }
 
/* If end of file when non-interactive */
         
    } else {
        exit(0);
    }  
}


/*  Returns to the top page of the present node
**  ===========================================
**
**  Prints the first page of the present node again, so the user can go back
**  down through the document.
*/

#ifdef __STDC__
PUBLIC void HTTopofNode(void)
#else
PUBLIC void HTTopofNode()
#endif

{
    output_ptr = head;

    num_of_lines = 1;

    HTOutputLines();
 
    Selection_Prompt();
}


 
