/*                                                             HTBufferFile.c
**                                                                3-7-91
** A module to be called from the WWW Line Mode Browser program WWW.c, to read 
** the contents into a file at the start of the program, so that the server is 
** freed as quickly as possible.
*/

#include "tcp.h"		/* defines SHORT_NAMES if necessary */
#ifdef SHORT_NAMES
#include "HTShort.h"
#endif

#include "HTUtils.h"
#include "WWW.h"
#include "HTBufferFile.h"
#include "HTAccess.h"



/*  Module Compilation Parameters
**  -----------------------------
*/
#define BUFFER_SIZE 1024

#ifndef EOF
#define EOF (-1)                       /* End of file character defined as -1 */
#endif

/*  Module-Wide Types
**  -----------------
*/
    typedef struct input_buffer {
  
                   struct input_buffer  *next;

		   int			chars;             /* Number of chars.. 
                                                                in the buffer */
                   char                 text[BUFFER_SIZE];


            } file_buffer ;


/*  Module-Wide Variables
**  ---------------------
*/

PRIVATE file_buffer          *head = 0,
                             *tail = 0;           
PRIVATE char	             *read_pointer;
PRIVATE char	             *read_limit; 
PRIVATE int	              file_number = (-1);   /* Invalid initially */


/*  Free Off All Buffers
**  ====================
**
**  Called when the buffered input is no longer neeed.
**
**  On entry,
**	No constraints. Buffers may or may not have been allocated
**  On exit,
**	Read pointers are initialised, all buffers are freed.
*/

#ifdef __STDC__
PUBLIC void HTBufferClose(void)
#else
PUBLIC void HTBufferClose()
#endif

{

    while (head) {
	file_buffer * temp = head->next;
	free(head);
	head = temp;
    }

    head = tail = 0;
    read_pointer = read_limit = 0;                  /* Force read of buffer or
                                                       file */
}



/*   Open File Buffers
**   =================
**
**   The file number is stored for future use.
**   
**   On entry,
**        f_n           The number already previously allocated to the file on 
**                      accessing. 
**   On exit,
**	  HTNextChar	may be called to access the file.
*/

#ifdef __STDC__ 
PUBLIC void HTBufferOpen(int f_n)
#else
PUBLIC void HTBufferOpen(f_n) 
    int f_n;
#endif

{
    if (file_number>=0) HTClose(file_number);          /* Closes file and 
                                                          frees server */
    file_number = f_n;
    HTBufferClose();	     /* In case caller forgot */ 
}


    
/*   Reads Part of File into a Buffer
**   ================================
**
**   Always allocates a buffer, may be empty.
**
**   Returns
**	<= 0 	Error or EOF
**	 > 0	Number of chars
*/

#ifdef __STDC__
PRIVATE int HTBufferRead(void)
#else
PRIVATE int HTBufferRead()
#endif

{
    int     status;
    PRIVATE  file_buffer *ptr;	    /* New element */

    ptr = (file_buffer *)malloc(sizeof(file_buffer));
    ptr->chars = 0;
    ptr->next = 0;

    if (head==0){
	head = ptr;		/* First element */
    } else {
	tail->next = ptr;	/* Not first element */
    }
    tail = ptr;

    while (ptr->chars < BUFFER_SIZE) {

        if (file_number<0)
            return 0;                  /* End of file - none open @@ */
        status = NETREAD(file_number,
			ptr->text + ptr->chars,
			BUFFER_SIZE - ptr->chars); 

	if (status <= 0){
            HTClose(file_number);
            file_number = -1;         /* Remember its closed */
            return status;	      /* Error or EOF */
        }
#ifdef NOT_ASCII
	{				/* Convert from ASCII to EBCDIC */
	    char *p;
	    char * start = ptr->text + ptr->chars;
	    char * end   = start + status;
	    for (p = start; p < end; p++) {
		*p = FROMASCII(*p);
	    }
	}
#endif
        ptr->chars = ptr->chars + status;
    }

    return ptr->chars;
}



/*   Reads All the Remaining File into a Linked List Buffer
**   ======================================================
*/

#ifdef __STDC__
PUBLIC void HTBufferFile(void)
#else
PUBLIC void HTBufferFile()
#endif

{
    int     status;

    while ((status = HTBufferRead()) > 0 )   /* Loop */;
}



/*   Read a Character From the Buffer
**   ================================
**
**   This is only necessary because on a vax, we can't make a file
**   number into a file pointer, so we can't use getc().
**
**   On exit,
**
**	  returns
**		-1	Error or eof
**		char	success
*/

#ifdef __STDC__
PUBLIC char HTNextChar(void)
#else
PUBLIC char HTNextChar()
#endif

{

    if (read_pointer == read_limit){	    /* Anything left in THIS buffer? */

        if (head) {			    /* If not, free it */
	    file_buffer *temp;
	    temp = head;
            head = head->next;
            free(temp);
	}

	if (head == 0) {		    /* Any more buffers ? */
	    HTBufferRead();                 /* If not, get one */
        }

        if (head->chars == 0) return EOF;   /* None left even in file */ 

        read_pointer = head->text;
        read_limit = head->text + head->chars;
    }

    return *read_pointer++;

}



/*   Back Up One Character
**   =====================
*/

#ifdef __STDC__
PUBLIC char HTBackUp(void)
#else
PUBLIC char HTBackUp()
#endif

{
    return *read_pointer--;
}



