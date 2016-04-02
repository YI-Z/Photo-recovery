/****************************************************************************
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 ***************************************************************************/
 #include <stdio.h>
 #include <stdlib.h>
 #include <stdint.h>
 #define MAXIUM_IMAGE_NUM 100
 typedef uint8_t  BYTE;
 
 int main(void)
 {  
    
    //open the raw file and read in raw pointer
    FILE* raw=fopen("card.raw","r");
    if (raw==NULL)
        return 1;
    
    int index=0;  //initialize the index showing the ith block
    int n=0;  //initialize the number of pictures: actual number will be n
    int starter_index[MAXIUM_IMAGE_NUM];  //initialize the index number of blocks containing starters
    while(!feof(raw))
    {
        BYTE block[512*sizeof(BYTE)];  //buffer block, FAT system, data stored in 512-byte blocks
        fread(block,sizeof(BYTE),512,raw);
        
   
        //find the starter sequence in each block and mark in the starter_index 
        if (block[0]==0xff && block[1]==0xd8 && block[2]==0xff && (block[3]==0xe0 || block[3]==0xe1))
        {
            starter_index[n]=index;
            n++;
        }
        index++;
           
    }
    //set the cursor to the very begining of the first jpeg image
    fseek(raw,512*starter_index[0]*sizeof(BYTE),SEEK_SET);
    
    //write data into each jpeg files, this only write n images, the last one doesn't have a
    //starter sequence, so it will be considered as from the last starter to the end of the file
    for (int i=0;i<n-1;i++)
    {
        char outfile_name[(7+1)*sizeof(char)];  //this is very important to add an extra space for the null character
        sprintf(outfile_name,"%03d.jpg",i);  //get the name for each file
        FILE* outfile_ptr=fopen(outfile_name,"w");
        if (outfile_ptr==NULL)
            return 2;
           
        int image_size=512*(starter_index[i+1]-starter_index[i])*sizeof(BYTE);  //the actual size of each image
        BYTE buffer[image_size];
        fread(buffer,sizeof(BYTE),image_size,raw);
        fwrite(buffer,sizeof(BYTE),image_size,outfile_ptr);
        fclose(outfile_ptr);
    }
    
    //write the last file
    char outfile_name[7];
    sprintf(outfile_name,"%03d.jpg",n-1);  //open and creat the last file
    FILE* outfile_ptr=fopen(outfile_name,"w");
    if (outfile_ptr==NULL)
        return 2;
   
    
    BYTE buffer;
    while (!feof(raw))
    {
        fread(&buffer,sizeof(BYTE),1,raw);
        fwrite(&buffer,sizeof(BYTE),1,outfile_ptr);
    }
    fclose(outfile_ptr);   
    
    //close raw file and exit
    fclose(raw);
    
    return 0; 
    
 }
