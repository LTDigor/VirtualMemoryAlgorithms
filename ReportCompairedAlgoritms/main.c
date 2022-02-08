#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <windows.h>

int pageFaultsOPT(const int, const int, int * const);
int pageFaultsLRU(const int, const int, int * const);
int pageFaultsFIFO(const int, const int, int * const);
int pageFaultsCLOCK(const int, const int, int * const);

int pageInFrames(int * const, const int, const int);

void testOPT();
void testLRU();
void testFIFO();
void testCLOCK();

void generatePages(const int , const int , int *);

int main()
{
    const int MaxPageNumber = 100000, MaxPageValue = 1000;
    const int MaxCalcNumber = 10;//number of calculations with difference data
    const int PointsPage = 30, PointsFrames = 30, PointsMaxPageValue = 30; //the number of calculations when finding dependencies
    const int StandartFrameNumber = 30;//the default number of frames in the study
    const int StandartPageValue = 150;//the default number of max page number in the study
    const int TimeRepeatNumber = 30;

    int resultOPT, resultFIFO, resultLRU, resultCLOCK;
    int i, j, pageNumber = MaxPageNumber, frameNumber = StandartFrameNumber, pageValue = StandartPageValue;
    double timeOPT, timeFIFO, timeLRU, timeCLOCK;
    int **pages;
    FILE *fFaults_PageNumber, *fTime_PageNumber, *fFaults_Frames, *fMaxPageValue_Faults;

    //test functions
    testOPT();
    testFIFO();
    testLRU();
    testCLOCK();
    //open files
    if((fFaults_PageNumber = fopen("src/Faults_PageNumber.txt", "w")) == NULL)
        exit(-2);
    if((fTime_PageNumber = fopen("src/Time_PageNumber.txt", "w")) == NULL)
        exit(-2);
    if((fFaults_Frames = fopen("src/Faults_Frames.txt", "w")) == NULL)
        exit(-2);
    if((fMaxPageValue_Faults = fopen("src/Faults_MaxPageValue.txt", "w")) == NULL)
        exit(-2);

    //create array with test data
    pages =(int **) malloc(MaxCalcNumber * sizeof(int*));
    if (pages == NULL)
        exit(-2);//cant find memory

    //generate data of page arrays
    for(i = 0; i < MaxCalcNumber; i++)
    {
        pages[i] =(int*) malloc(MaxPageNumber * sizeof(int));
        if(pages[i] == NULL)
            exit(-2);//cant find memory

        //fill the array of pages
        generatePages(StandartPageValue, MaxPageNumber, pages[i]);
    }

     //dependences Faults(pageNumber)
     //framesNumber = StandartFrameNumber and max page value = StandartPageValue
    frameNumber = StandartFrameNumber;
    for(pageNumber = 1; pageNumber <= MaxPageNumber; pageNumber += MaxPageNumber / PointsPage)
    {
        resultOPT = 0;
        resultFIFO = 0;
        resultLRU = 0;
        resultCLOCK = 0;
        for(i = 0; i < MaxCalcNumber; i++)
        {
            resultOPT += pageFaultsOPT(frameNumber, pageNumber, pages[i]);
            resultFIFO += pageFaultsFIFO(frameNumber, pageNumber, pages[i]);
            resultLRU += pageFaultsLRU(frameNumber, pageNumber, pages[i]);
            resultCLOCK += pageFaultsCLOCK(frameNumber, pageNumber, pages[i]);
        }

        resultOPT /= MaxCalcNumber;
        resultFIFO /= MaxCalcNumber;
        resultLRU /= MaxCalcNumber;
        resultCLOCK /= MaxCalcNumber;

        fprintf(fFaults_PageNumber,  "%i %i %i %i %i\n",
                        pageNumber, resultOPT, resultFIFO, resultLRU, resultCLOCK);
    }
    printf("Faults(pageNumber) done!\n");

    //dependences time(pageNumber)
    //with framesNumber = StandartFrameNumber and max page value = StandartPageValue
    for(pageNumber = 1; pageNumber <= MaxPageNumber; pageNumber += MaxPageNumber / PointsPage)
    {
        timeOPT = 0;
        timeFIFO = 0;
        timeLRU = 0;
        timeCLOCK = 0;
        for(i = 0; i < MaxCalcNumber; i++)
        {
            timeOPT -= clock();
            for(j = 0; j < TimeRepeatNumber; j++)
                pageFaultsOPT(frameNumber, pageNumber, pages[i]);
            timeOPT += clock();

            timeFIFO -= clock();
            for(j = 0; j < TimeRepeatNumber; j++)
                pageFaultsFIFO(frameNumber, pageNumber, pages[i]);
            timeFIFO += clock();

            timeLRU -= clock();
            for(j = 0; j < TimeRepeatNumber; j++)
                pageFaultsLRU(frameNumber, pageNumber, pages[i]);
            timeLRU += clock();

            timeCLOCK -= clock();
            for(j = 0; j < TimeRepeatNumber; j++)
                pageFaultsCLOCK(frameNumber, pageNumber, pages[i]);
            timeCLOCK += clock();

        }

        timeOPT /= MaxCalcNumber * TimeRepeatNumber;
        timeFIFO /= MaxCalcNumber * TimeRepeatNumber;
        timeLRU /= MaxCalcNumber * TimeRepeatNumber;
        timeCLOCK /= MaxCalcNumber * TimeRepeatNumber;

        fprintf(fTime_PageNumber,  "%i %lf %lf %lf %lf\n", pageNumber,
                timeOPT/CLK_TCK, timeFIFO/CLK_TCK, timeLRU/CLK_TCK, timeCLOCK/CLK_TCK);
    }
    printf("time(pageNumber) done!\n");

    //dependences Faults(frameNumber)
    //with page number = MaxPageNumber and max page value = StandartPageValue
    pageNumber = MaxPageNumber;
    for(frameNumber = 1; frameNumber < StandartPageValue; frameNumber += StandartPageValue / PointsFrames)
    {
        resultOPT = 0;
        resultFIFO = 0;
        resultLRU = 0;
        resultCLOCK = 0;
        for(i = 0; i < MaxCalcNumber; i++)
        {
            resultOPT += pageFaultsOPT(frameNumber, pageNumber, pages[i]);
            resultFIFO += pageFaultsFIFO(frameNumber, pageNumber, pages[i]);
            resultLRU += pageFaultsLRU(frameNumber, pageNumber, pages[i]);
            resultCLOCK += pageFaultsCLOCK(frameNumber, pageNumber, pages[i]);
        }

        resultOPT /= MaxCalcNumber;
        resultFIFO /= MaxCalcNumber;
        resultLRU /= MaxCalcNumber;
        resultCLOCK /= MaxCalcNumber;

        fprintf(fFaults_Frames,  "%i %i %i %i %i\n",
                        frameNumber, resultOPT, resultFIFO, resultLRU, resultCLOCK);
    }
    printf("Faults(frameNumber) done!\n");

    //dependences Faults(maxPageValue)
    //with page number = MaxPageNumber and frames number = StandartFrameNumber
    frameNumber = StandartFrameNumber;
    for(pageValue = 10; pageValue < MaxPageValue; pageValue += MaxPageValue / PointsMaxPageValue)
    {
        for(i = 0; i < MaxCalcNumber; i++)
            generatePages(pageValue, MaxPageNumber, pages[i]);

        resultOPT = 0;
        resultFIFO = 0;
        resultLRU = 0;
        resultCLOCK = 0;
        for(i = 0; i < MaxCalcNumber; i++)
        {
            resultOPT += pageFaultsOPT(frameNumber, MaxPageNumber, pages[i]);
            resultFIFO += pageFaultsFIFO(frameNumber, MaxPageNumber, pages[i]);
            resultLRU += pageFaultsLRU(frameNumber, MaxPageNumber, pages[i]);
            resultCLOCK += pageFaultsCLOCK(frameNumber, MaxPageNumber, pages[i]);
        }

        resultOPT /= MaxCalcNumber;
        resultFIFO /= MaxCalcNumber;
        resultLRU /= MaxCalcNumber;
        resultCLOCK /= MaxCalcNumber;

        fprintf(fMaxPageValue_Faults, "%i %i %i %i %i\n",
                        pageValue, resultOPT, resultFIFO, resultLRU, resultCLOCK);
    }
    printf("Faults(maxPageValue) done!\n");

    //free memory
    for(i = 0; i < MaxCalcNumber; i++)
        free(pages[i]);
    free(pages);

    //close files
    fclose(fFaults_PageNumber);
    fclose(fTime_PageNumber);
    fclose(fFaults_Frames);
    fclose(fMaxPageValue_Faults);

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
    int frameIndex, pageIndex;

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
            frames[replacedFrameIndex] = pages[pageIndex];
            replacedFrameIndex = (replacedFrameIndex + 1) % frameNumber;
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

void generatePages(const int maxPageValue, const int pageNumber, int *pages)
{
    int i;

    srand(time(NULL));
    for (i = 0; i < pageNumber; i++)
    {
        pages[i] = rand() % maxPageValue;
    }
}
