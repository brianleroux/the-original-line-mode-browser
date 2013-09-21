/*              Stores the Formatted Text in a Buffer           HTFormatText.h  
**              ====================================               3-7-91
**
** Storing the formatted text in a buffer will be particulary useful for
** improving the paging code so that it is more sensitive towards things like
** headings appearing at the bottom of pages. It also can be used for a
** scrolling system aswell outputting the text into a file rather than
** straight onto the screen.
*/


/*  Frees Off Formatted Buffer Before Taking in a New File
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
extern void HTFormatBufferClose(void);
#else
extern void HTFormatBufferClose();
#endif


/*  Reads Each Formatted Line into a Double Linked List
**  ===================================================
**
**  Called everytime a new formatted line is produced.
**
**  On entry,
**	
**  On exit,
**
*/
#ifdef __STDC__
extern void HTBufferFormattedLine(char * formatted_line);
#else
extern void HTBufferFormattedLine();
#endif


/*  Outputs the Formatted Buffer
**  ============================
**
**  Outputs the formatted text from a double linked list buffer, one page at a  
**  time. Checks to see if there is enough formatted text in the buffer to 
**  produce a complete page. If not, calls Format_Text() to send to the buffer
**  more formatted characters.
**
*/
#ifdef __STDC__
extern void HTOutputLines(void);
#else
extern void HTOutputLines();
#endif


/*  Prints on the Screen the Contents of the Buffer
**  ===============================================
**  
**  Checks the layout of the text, looking at the vertical spacing, preventing
**  titles appearing on one page and the corresponding text on the next.
*/
#ifdef _STDC_
extern void HTOutputBufferContent(void);
#else
extern void HTOutputBufferContent();
#endif


/*  At the End of the Document Inserts Blank Lines to Tidy Screen
**  =============================================================
*/
#ifdef _STDC_
extern void HTEndofDocument(void);
#else
extern void HTEndofDocument();
#endif


/*  Returns to the top page of the present node
**  ===========================================
**
**  Prints the first page of the present node again, so the user can go back
**  down through the document.
*/
#ifdef __STDC__
extern void HTTopofNode(void);
#else
extern void HTTopofNode();
#endif
