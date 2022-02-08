clear
function drawGrapsFromFile(filename, graphName, winNum)
    pages = []
    opt = []
    fifo = []
    lru = []
    clk = []

    f = mopen(filename, 'r')
    i = 0;
    while meof(f) == 0
        i = i + 1
        [n, pages(i), opt(i), fifo(i), lru(i), clk(i)] =...
         mfscanf(f, "%i %lf %lf %lf %lf")
    end
    
    scf(winNum)
    title(graphName)
    plot2d(pages, [opt, fifo, lru, clk], [2 , 3, 4, 5])
    legend(['OPT', 'FIFO', 'LRU', 'CLOCK'], -4)
    mclose(f)
    
    //Export
    image_name = strsubst(filename, '.txt', '.png')
    xs2png(winNum, image_name)
endfunction

// Построение графиков из файлов
drawGrapsFromFile('src/Time_PageNumber.txt', 'time(pageNumber)', 1)
drawGrapsFromFile('src/Faults_Frames.txt', 'pageFaults(frameNumber)', 2)
drawGrapsFromFile('src/Faults_PageNumber.txt', 'pageFaults(pageNumber)', 3)
drawGrapsFromFile('src/Faults_MaxPageValue.txt', 'pageFaults(maxPageValue)', 4)