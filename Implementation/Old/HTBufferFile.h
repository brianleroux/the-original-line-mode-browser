/*              Buffer File and Return Pointer                 HTBufferFile.h  
**              ==============================                    19-4-91
**
**  The current file accessed needs to be almost immediately read into a
**  buffer once opened so that the server is freed as soon as possible.
*/

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
extern  void HTBufferClose(void);
#else
extern  void HTBufferClose();
#endif


/*   Open File Buffers
**   =================
**
**   The file number is stored for future use.
**   
**   On entry,
**        f_n   The file number already previously allocated to the file on 
**                      accessing.
**   On exit,
**	  HTNextChar	may be called to access the file.
*/
#ifdef __STDC__
extern  void HTBufferOpen(int f_n);
#else
extern  void HTBufferOpen();
#endif 



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
extern  int  HTBufferRead(void);
#else
extern  int  HTBufferRead();
#endif


/*   Reads All the Remaining File into a Linked List Buffer
**   ======================================================
*/

#ifdef __STDC__
extern  void  HTBufferFile(void);
#else
extern  void  HTBufferFile();
#endif


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
extern  char  HTNextChar(void);
#else
extern  char  HTNextChar();
#endif 

#define NEXT_CHAR HTNextChar()


/*   Back Up One Character
**   =====================
*/

#ifdef __STDC__
extern   char  HTBackUp(void);
#else
extern   char  HTBackUp();
#endif

#define BACK_UP HTBackUp()

