//                  DESCRIPTION:
// The number of frames available to the process is fixed.
// When a process needs some page, the operating system must make sure
// that it is loaded into RAM. The situation when there is no page in RAM
// at the time of the request is called a page fault.
// At this moment, the operating system should load it into one of the allocated process
// frames of RAM. If there are no free frames, then the operating system must
// unload one of the previously loaded pages and load the required one in its place.
// To determine the page to be unloaded, the operating system uses the
// page replacement algorithm. This program implements and compares
// several basic substitution algorithms that form the basis of those algorithms
// that are used in practice.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int pageFaultsOPT(const int, const int, int * const);
int pageFaultsLRU(const int, const int, int * const);
int pageFaultsFIFO(const int, const int, int * const);
int pageFaultsCLOCK(const int, const int, int * const);

int pageInFrames(int * const, const int, const int);

void testOPT();
void testLRU();
void testFIFO();
void testCLOCK();

int main()
{
    int resultOPT, resultFIFO, resultLRU, resultCLOCK;
    int frameNumber, pageNumber, i;
    int *pages;

    //test functions
    testOPT();
    testFIFO();
    testLRU();
    testCLOCK();

    scanf("%i %i", &frameNumber, &pageNumber);
    pages =(int *) malloc(pageNumber * sizeof(int));

    if (pages == NULL)
        exit(0);//cant find memory;

    for (i = 0; i < pageNumber; i++)
        scanf("%i", pages + i);

    //find results
    resultOPT = pageFaultsOPT(frameNumber, pageNumber, pages);
    resultFIFO = pageFaultsFIFO(frameNumber, pageNumber, pages);
    resultLRU = pageFaultsLRU(frameNumber, pageNumber, pages);
    resultCLOCK = pageFaultsCLOCK(frameNumber, pageNumber, pages);

    printf("OPT: %i\n", resultOPT);
    printf("FIFO: %i\n", resultFIFO);
    printf("LRU: %i\n", resultLRU);
    printf("CLOCK: %i\n", resultCLOCK);

    free(pages);

    return 0;
}

//the function returns the index of the first frame in which there is a page
//and -1 if there is no page among the frames
int pageInFrames(int * const frames, const int frameNumber, int page)
{
    int pageIndex = -1, frameIndex;

    for (frameIndex = 0; frameIndex < frameNumber; frameIndex++)
    {
        if (page == frames[frameIndex])
        {
            pageIndex = frameIndex;
            break;
        }
    }

    return pageIndex;
}

int pageFaultsOPT(const int frameNumber, const int pageNumber, int * const pages)
{
    //the array whenNeed stores the values after how many steps a frame will be needed
    int frames[frameNumber], whenNeed[frameNumber];
    //replacedFrameIndex is the index of the frame will be replaced by the current page
    int faultCaunter = 0, replacedFrameIndex = 0;
    int frameIndex, pageIndex, i;

    //fill the array of frames; assume that the pages are positive
    for (int i = 0; i < frameNumber; i++)
    {
        frames[i] = -1;
        whenNeed[i] = 0;
    }

    //processing each page
    for (pageIndex = 0; pageIndex < pageNumber; pageIndex++)
    {
        //check if the current page is already in the frames array
        replacedFrameIndex = pageInFrames(frames, frameNumber, pages[pageIndex]);

        //load the page in the frames array
        if (replacedFrameIndex == -1)
        {
            //find the index of the page in the frameArray that will not be needed the longest
            for (frameIndex = 0; frameIndex < frameNumber; frameIndex++)
            {
                //page will never use again or whenNeed[frameIndex] is 0
                if( whenNeed[frameIndex] == 0 || whenNeed[frameIndex] > whenNeed[replacedFrameIndex])
                {
                    replacedFrameIndex = frameIndex;
                }

                if( whenNeed[replacedFrameIndex] == 0 )//the best item
                    break;
            }

            frames[replacedFrameIndex] = pages[pageIndex];
            faultCaunter++;
        }

        //calculate when the page in the frame will be needed again
        //0 in whenNeed means that the page in the frame will no longer be needed.
        whenNeed[replacedFrameIndex] = 0;
        for (int nextPageIndex = pageIndex + 1; nextPageIndex < pageNumber; nextPageIndex++)
        {
            if (frames[replacedFrameIndex] == pages[nextPageIndex])
            {
                whenNeed[replacedFrameIndex] = nextPageIndex - pageIndex;
                break;
            }
        }
    }

    return faultCaunter;
}

int pageFaultsFIFO(const int frameNumber, const int pageNumber, int * const pages)
{
    int frames[frameNumber];
    //replacedFrameIndex is the index of the frame will be replaced by the current page
    int faultCaunter = 0, replacedFrameIndex = 0;
    int pageIndex, i;

    //fill the array of frames; assume that the pages are positive
    for (i = 0; i < frameNumber; i++)
        frames[i] = -1;

    //processing each page
    for (pageIndex = 0; pageIndex < pageNumber; pageIndex++)
    {
        //check if the current page is already in the frames array
        //load the page in the frames array
        if (pageInFrames(frames, frameNumber, pages[pageIndex]) == -1)
        {
            //next index in the frame array considering overflow
            replacedFrameIndex = (replacedFrameIndex + 1) % frameNumber;
            frames[replacedFrameIndex] = pages[pageIndex];
            faultCaunter++;
        }
    }

    return faultCaunter;
}

int pageFaultsLRU(const int frameNumber, const int pageNumber, int * const pages)
{
    int frames[frameNumber], cache[frameNumber];
    //replacedFrameIndex is the index of the frame will be replaced by the current page
    int faultCaunter = 0, replacedFrameIndex = 0;
    int pageIndex, i;

    //fill the array of frames; assume that the pages are positive
    for (i = 0; i < frameNumber; i++)
    {
        frames[i] = -1;
        cache[i] = 0;
    }

    //processing each page
    for (pageIndex = 0; pageIndex < pageNumber; pageIndex++)
    {
        //increase the cache
        for (i = 0; i < frameNumber; i++)
            cache[i]++;

        //check if the current page is already in the frames array
        replacedFrameIndex = pageInFrames(frames, frameNumber, pages[pageIndex]);

        //page is already in frames
        if (replacedFrameIndex != -1)
            cache[replacedFrameIndex] = 0;
        else //load the page in the frames array
        {
            //LRU, find the index of the frame that will be replaced (item with max cache)
            for (i = 0; i < frameNumber; i++ )
                if (cache[i] > cache[replacedFrameIndex] )
                    replacedFrameIndex = i;

            frames[replacedFrameIndex] = pages[pageIndex];
            cache[replacedFrameIndex] = 0;
            faultCaunter++;
        }
    }

    return faultCaunter;
}

int pageFaultsCLOCK(const int frameNumber, const int pageNumber, int * const pages)
{
    int frames[frameNumber], clock[frameNumber];
    //replacedFrameIndex is the index of the frame will be replaced by the current page
    int indexInFrames = 0, faultCaunter = 0, replacedFrameIndex = 0;
    int currentIndex, pageIndex, i;

    //fill the array of frames; assume that the pages are positive
    for (i = 0; i < frameNumber; i++)
    {
        frames[i] = -1;
        clock[i] = 0;
    }

    //processing each page
    for (pageIndex = 0; pageIndex < pageNumber; pageIndex++)
    {
        //check if the current page is already in the frames array
        indexInFrames = pageInFrames(frames, frameNumber, pages[pageIndex]);

        //page is already in frames
        if (indexInFrames != -1)
            clock[indexInFrames] = 1;
        else //load the page in the frames array
        {
            //LRU, find the index of the frame that will be replaced (item with max cache)
            for (i = 0; i < frameNumber; i++ )
            {
                //index of the next frame for calculating
                currentIndex = (replacedFrameIndex + i) % frameNumber;
                if (clock[currentIndex] == 0)//the best frame for replace
                {
                    replacedFrameIndex = currentIndex;
                    break;
                }
                else
                    clock[currentIndex] = 0;//visited and not replaced frame
            }

            frames[replacedFrameIndex] = pages[pageIndex];
            clock[replacedFrameIndex] = 1;
            //next index in the frame array considering overflow
            replacedFrameIndex = (replacedFrameIndex + 1) % frameNumber;
            faultCaunter++;
        }
    }

    return faultCaunter;
}

void testOPT()
{
    int testData1[7] = {1, 2, 3, 4, 2, 3, 1};
    int testData2[12] = {0, 1, 2, 3, 0, 1, 4, 0, 1, 2, 3, 4};
    int testData3[1] = {1};

    assert(pageFaultsOPT(3, 7, testData1) == 5);
    assert(pageFaultsOPT(3, 12, testData2) == 7);
    assert(pageFaultsOPT(4, 12, testData2) == 6);
    assert(pageFaultsOPT(2, 1, testData3) == 1);
}

void testFIFO()
{
    int testData1[7] = {1, 2, 3, 4, 2, 3, 1};
    int testData2[12] = {0, 1, 2, 3, 0, 1, 4, 0, 1, 2, 3, 4};
    int testData3[1] = {1};

    assert(pageFaultsFIFO(3, 7, testData1) == 5);
    assert(pageFaultsFIFO(3, 12, testData2) == 9);
    assert(pageFaultsFIFO(4, 12, testData2) == 10);
    assert(pageFaultsFIFO(2, 1, testData3) == 1);
}

void testLRU()
{
    int testData1[7] = {1, 2, 3, 4, 2, 3, 1};
    int testData2[12] = {0, 1, 2, 3, 0, 1, 4, 0, 1, 2, 3, 4};
    int testData3[1] = {1};

    assert(pageFaultsLRU(3, 7, testData1) == 5);
    assert(pageFaultsLRU(3, 12, testData2) == 10);
    assert(pageFaultsLRU(4, 12, testData2) == 8);
    assert(pageFaultsLRU(2, 1, testData3) == 1);
}

void testCLOCK()
{
    int testData1[7] = {1, 2, 3, 4, 2, 3, 1};
    int testData2[12] = {0, 1, 2, 3, 0, 1, 4, 0, 1, 2, 3, 4};
    int testData3[1] = {1};

    assert(pageFaultsCLOCK(3, 7, testData1) == 5);
    assert(pageFaultsCLOCK(3, 12, testData2) == 9);
    assert(pageFaultsCLOCK(4, 12, testData2) == 10);
    assert(pageFaultsCLOCK(2, 1, testData3) == 1);
}
