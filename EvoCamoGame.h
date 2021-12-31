//
//  EvoCamoGame.h
//  TexSyn
//
//  Created by Craig Reynolds on 1/20/21.
//  Copyright © 2021 Craig Reynolds. All rights reserved.
//
//------------------------------------------------------------------------------
//
// "Interactive Evolution of Camouflage" implemented on top of TexSyn and
// LazyPredator. Slightly updated version of the approach used in:
//
//     Craig Reynolds. 2011. Interactive Evolution of Camouflage.
//     Artificial Life 17(2). https://doi.org/10.1162/artl_a_00023
//
// This class is the top level "main()" for unix-style command evo_camo_game.
// See doc: https://cwreynolds.github.io/TexSyn/evo_camo_game_doc.html
//
// Prototyping inside TexSyn, maybe should be its own library/git repository.
//
//------------------------------------------------------------------------------

#pragma once
#include "GP.h"

class EvoCamoGame
{
public:
    // Constructor to get parameters from pre-parsed "unix style" command line.
    EvoCamoGame(const CommandLine& cmd)
      : run_name_(runNameDefault(cmd)),
        background_image_directory_(cmd.positionalArgument(1)),
        output_directory_(cmd.positionalArgument(2, ".")),
        output_directory_this_run_(runOutputDirectory()),
        background_scale_(cmd.positionalArgument(3, float(0.5))),
        random_seed_(cmd.positionalArgument(4, int(LPRS().defaultSeed()))),
        gui_size_(cmd.positionalArgument(5, 1200),
                  cmd.positionalArgument(6, 800)),
        gui_(gui_size_, Vec2(), run_name_),
        individuals_(cmd.positionalArgument(7, 120)),
        subpops_(cmd.positionalArgument(8, 6)),
        max_init_tree_size_(cmd.positionalArgument(9, 100)),
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        min_crossover_tree_size_(max_init_tree_size_ * 0.5),
//        max_crossover_tree_size_(max_init_tree_size_ * 1.5),
        min_crossover_tree_size_
            (cmd.positionalArgument(10, max_init_tree_size_ * 0.5f)),
        max_crossover_tree_size_
            (cmd.positionalArgument(11, max_init_tree_size_ * 1.5f))
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    {
        if ((backgroundScale() > 10000) || (backgroundScale() <  0.0001))
        {
            // Better warning for confusing case: the output_directory arg was
            // accidentally omitted, scale read a value (20210529) which was WAY
            // too large and caused a confusing assert fail deep inside OpenCV.
            debugPrint(backgroundScale());
            assert(!"backgroundScale() seems out of range");
        }
        if (background_image_directory_.empty())
        {
            // Exit with failure after listing command arguments.
            std::cout << cmd.commandNameWithoutPath();
            std::cout << " requires at least one pathname parameter,";
            std::cout << " others may be omitted from the end:" << std::endl;
            std::cout << "    background_image_directory (required)"<<std::endl;
            // TODO this should say something about creating a time-stamped
            // directory under this for output from this run.
            std::cout << "    output_directory (defaults to .)" << std::endl;
            std::cout << "    background_scale (defaults to 0.5)" << std::endl;
            std::cout << "    random_seed (else: default seed)" << std::endl;
            std::cout << "    window width (defaults to 1200)" << std::endl;
            std::cout << "    window height (defaults to 800)" << std::endl;
            std::cout << "    individuals (defaults to 120)" << std::endl;
            std::cout << "    subpopulations (defaults to 6)" << std::endl;
            std::cout << "    max_init_tree_size (defaults to 100)"<< std::endl;
            std::cout << "    min_crossover_tree_size "
                         "(default max_init_tree_size_ * 0.5)" << std::endl;
            std::cout << "    max_crossover_tree_size "
                         "(default max_init_tree_size_ * 1.5)" << std::endl;
            std::cout << std::endl;
            exit(EXIT_FAILURE);
        }
        else
        {
            std::cout << "Interactive evolution of camouflage:" << std::endl;
            debugPrint(run_name_);
            debugPrint(background_image_directory_);
            debugPrint(output_directory_);
            debugPrint(output_directory_this_run_);
            debugPrint(background_scale_);
            debugPrint(random_seed_);
            debugPrint(gui_.getSize());
            debugPrint(individuals_);
            debugPrint(subpops_);
            debugPrint(max_init_tree_size_);
            debugPrint(min_crossover_tree_size_);
            debugPrint(max_crossover_tree_size_);
        }
    }
    
    // Read specified background image files, scale, and save as cv::Mats.
    void collectBackgroundImages()
    {
        // Names of all files in backgroundImageDirectory() (expect image files)
        const std::vector<std::string> background_image_filenames =
            directory_filenames(backgroundImageDirectory());
        std::cout << "Reading background images:" << std::endl;
        int min_x = std::numeric_limits<int>::max();
        int min_y = std::numeric_limits<int>::max();
        for (auto& filename : background_image_filenames)
        {
            // Compose absolute pathname for this background image file.
            std::string pathname = backgroundImageDirectory() + "/" + filename;
            // Read the image file into an OpenCV image.
            cv::Mat bg = cv::imread(pathname);
            // When valid image file. (To ignore README.txt, .DS_Store, etc.)
            if (cv::haveImageReader(pathname))
            {
                std::cout << "    " << pathname << std::endl;
                // Keep track of smallest image dimensions.
                if (min_x > bg.cols) { min_x = bg.cols; }
                if (min_y > bg.rows) { min_y = bg.rows; }
                // Adjust the size/resolution by "background_scale" parameter.
                cv::resize(bg, bg,
                           cv::Size(), backgroundScale(), backgroundScale(),
                           cv::INTER_CUBIC);
                // Add to collection of background images.
                addBackgroundImage(bg);
            }
        }
        std::cout << "Found " << backgroundImages().size();
        std::cout << " background images." << std::endl;
        assert(!backgroundImages().empty());
        checkBackgroundImageSizes(min_x, min_y);
    }
    
    // Verify all background images (as scaled) are larger than GUI window.
    void checkBackgroundImageSizes(int min_x, int min_y)
    {
        float s = backgroundScale();
        int s_min_x = s * min_x;
        int s_min_y = s * min_y;
        if ((s_min_x < guiSize().x()) || (s_min_y < guiSize().y()))
        {
            std::cout << std::endl
                << "ERROR: BACKGROUND IMAGE IS TOO SMALL FOR WINDOW."
                << std::endl << "The smallest of " << backgroundImages().size()
                << " background images is " << min_x << "×" << min_y
                << " pixels." << std::endl << "Background scale " << s
                << " produces " << s_min_x << "×" << s_min_y << " image, "
                << "smaller than " << guiSize().x() << "×" << guiSize().y()
                << " window size." << std::endl
                << "Increase scale, reduce window size, or choose other images."
                << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    
    // Randomly select one of the given backgrounds, then randomly select a
    // window-sized rectangle within it.
    cv::Mat selectRandomBackgroundForWindow()
    {
        // Pick one of the given background images at random.
        const cv::Mat& bg = LPRS().randomSelectElement(backgroundImages());
        // Find how much bigger (than GUI window) the original background is.
        int dx = std::max(0, int(bg.cols - guiSize().x()));
        int dy = std::max(0, int(bg.rows - guiSize().y()));
        // Randomly select an offset within that size difference.
        Vec2 random_position(LPRS().randomN(dx), LPRS().randomN(dy));
        // Return a "submat" reference into the random rectangle inside "bg".
        return Texture::getCvMatRect(random_position, guiSize(), bg);
    }

    // Run the evolution simulation.
    void run()
    {
        LPRS().setSeed(random_seed_);
        std::cout << "Create initial population." << std::endl;
        setPopulation(std::make_shared<Population>(individuals_,
                                                   subpops_,
                                                   max_init_tree_size_,
                                                   min_crossover_tree_size_,
                                                   max_crossover_tree_size_,
                                                   GP::fs()));
        // Read specified background image files, save as cv::Mats.
        collectBackgroundImages();
        fs::path out = output_directory_this_run_;
        std::cout << "Create output directory for this run: ";
        std::cout << out << std::endl;
        fs::create_directory(out);
        // Init GUI window.
        gui().setWindowName(run_name_);
        gui().refresh();
        // Loop of interactive evolution steps, until "Q" command or forced exit.
//        running_ = true;
//        while (running_)
        setRunningState(true);
        while (getRunningState())
        {
            // Display step count in GUI title bar.
            std::string step_string = " (step " + getStepAsString() + ")";
            gui().setWindowTitle(run_name_ + step_string);
            logFunctionUsageCounts(out);
            // Evolution step with wrapped EvoCamoGame::tournamentFunction().
            getPopulation()->evolutionStep([&]
                                           (TournamentGroup tg)
                                           { return tournamentFunction(tg); });
        }
        // Delete Population instance.
        setPopulation(nullptr);
    }
    
    // TODO temporary utility for debugging random non-overlapping placement
    // TODO to be removed eventually
    void testdraw(const TournamentGroup& tg,
                  const std::vector<Disk>& disks,
                  Vec2 rect_min,
                  Vec2 rect_max,
                  float min_center_to_center)
    {
        int p = 0;
        gui().clear();
        gui().drawRectangle(rect_max - rect_min, rect_min, Color(0.6));
        for (auto& tgm : tg.members())
        {
            Texture* texture = GP::textureFromIndividual(tgm.individual);
            texture->rasterizeToImageCache(textureSize(), true);
            Vec2 center_to_ul = Vec2(1, 1) * textureSize() / 2;
            Vec2 position = disks.at(p++).position - center_to_ul;
            int size = textureSize();
            Vec2 size2(size, size);
            gui().drawCircle(min_center_to_center - size / 2,
                             disks.at(p-1).position,
                             Color(1));
            cv::Mat target = gui().getCvMatRect(position, size2);
            texture->matteImageCacheDiskOverBG(size, target);
        }
        gui().refresh();
        Texture::waitKey(10);
    }

    // TournamentFunction for "Interactive Evolution of Camouflage".
    TournamentGroup tournamentFunction(TournamentGroup tg)
    {
        // Restrict Texture disks to be completely inside a rectangle inset
        // from the window edge a Texture's radius. Rectangle defined by two
        // diagonally opposite corners.
        float radius = textureSize() / 2;
        Vec2 rect_min = Vec2(radius + 1, radius + 1);
        Vec2 rect_max = guiSize() - rect_min;
        // Find non-overlapping positions for the Textures in TournamentGroup.
        float margin = radius;
        auto overlap_viz = [&](const std::vector<Disk>& disks)
        {
            //testdraw(tg, disks, rect_min, rect_max, textureSize() + margin);
        };
        // Initialize "global variables" used by mouse callback handler.
        tournament_group_ = tg;
        background_image_ = selectRandomBackgroundForWindow();
        disks_ = Disk::randomNonOverlappingDisksInRectangle(3, radius, radius,
                                                            margin,
                                                            rect_min, rect_max,
                                                            LPRS(),
                                                            overlap_viz);
        // Draw the randomly selected background, then the 3 textures on top.
        gui().drawMat(background_image_, Vec2());
        drawTournamentGroupOverBackground(tg);
        // Update the onscreen image. Wait for user to click on one texture.
        TimePoint time_start_waiting = TimeClock::now();
        gui().refresh();
        setMouseCallbackForTournamentFunction();
        waitForUserInput();
        // Note time user took to respond. Ignored in logging of time per frame.
        getPopulation()->setIdleTime(TimeClock::now() - time_start_waiting);
        // Designate selected Texture's Individual as worst of TournamentGroup.
        Individual* worst = selectIndividualFromMouseClick(getLastMouseClick());
        tg.designateWorstIndividual(worst);
        // Clear GUI, return updated TournamentGroup.
        gui().clear();
        gui().refresh();
        return tg;
    }
    
    // Draw Textures of TournamentGroup over current background in GUI.
    void drawTournamentGroupOverBackground(const TournamentGroup& tg)
    {
        int p = 0;
        std::vector<Vec2> prey_texture_positions;
        for (auto& tgm : tg.members())
        {
            int size = textureSize();
            Texture* texture = GP::textureFromIndividual(tgm.individual);
            texture->rasterizeToImageCache(size, true);
            Vec2 center_to_ul = Vec2(1, 1) * size / 2;
            Vec2 position = disks_.at(p++).position - center_to_ul;
            prey_texture_positions.push_back(position);
            cv::Mat target = gui().getCvMatRect(position, Vec2(size, size));
            texture->matteImageCacheDiskOverBG(size, target);
        }
        // TODO 20211010 turn this off. If needed later add global enable flag.
        // writeTrainingSetData(prey_texture_positions);
    }

//        // Ad hoc idle loop, waiting for user input. Exits on left mouse click, the
//        // user's selection of the "worst" camouflage. This also "listens" for and
//        // executes single character commands: "t" and "Q".
//    //    void waitForUserInput()
//        // (20211230 make virtual so can be overridden, eg by EvoCamoVsStaticFCD)
//        virtual void waitForUserInput()
//        {
//            waitForMouseUp();  // In case mouse button still down from last click.
//    //        wait_for_mouse_click_ = true;
//            setWaitForMouseClick(true);
//            int previous_key = cv::waitKeyEx(1);
//            // Loop until mouse is clicked in window.
//    //        while (wait_for_mouse_click_)
//            while (getWaitForMouseClick())
//            {
//                // Wait for 1/4 second, and read any key typed during that time.
//                int key = cv::waitKey(250);  // 1/4 second (250/1000)
//                // When newly-pressed (key down) event.
//                if ((key > 0) && (key != previous_key))
//                {
//                    // For "t" command: write whole window tournament image to file.
//                    if (key == 't') { writeTournamentImageToFile(); }
//                    // For "Q" command: exit run() loop.
//                    if (key == 'Q')
//                    {
//                        running_ = false;
//                        wait_for_mouse_click_ = false;
//                        setWaitForMouseClick(false);
//                    }
//                }
//                previous_key = key;
//            }
//        }

    // Ad hoc idle loop, waiting for user input. Exits on left mouse click, the
    // user's selection of the "worst" camouflage. This also "listens" for and
    // executes single character commands: "t" and "Q".
    // (20211230 make virtual so can be overridden, eg by EvoCamoVsStaticFCD)
    virtual void waitForUserInput()
    {
        waitForMouseUp();  // In case mouse button still down from last click.
        setWaitForMouseClick(true);
        int previous_key = cv::waitKeyEx(1);
        // Loop until mouse is clicked in window.
        while (getWaitForMouseClick())
        {
            // Wait for 1/4 second, and read any key typed during that time.
            int key = cv::waitKey(250);  // 1/4 second (250/1000)
            // When newly-pressed (key down) event.
            if ((key > 0) && (key != previous_key))
            {
                // For "t" command: write whole window tournament image to file.
                if (key == 't') { writeTournamentImageToFile(); }
                // For "Q" command: exit run() loop.
                if (key == 'Q')
                {
//                    running_ = false;
                    setRunningState(false);
                    setWaitForMouseClick(false);
                }
            }
            previous_key = key;
        }
    }
    
    // Waits for mouse left button to be released (via mouse_left_button_down_).
    void waitForMouseUp()
    {
        // Wait (1/4 second at a time) until mouse_left_button_down_ is false.
        while (mouse_left_button_down_) { cv::waitKey(250); }
    }
    
    // Controls mouse behavior during a tournament.
    void setMouseCallbackForTournamentFunction()
    {
        auto mouse_callback =
        [](int event, int x, int y, int flags, void* userdata)
        {
            // Cast void* to EvoCamoGame*
            auto c = static_cast<EvoCamoGame*>(userdata);
            if (event == cv::EVENT_LBUTTONDOWN)  // Left button down.
            {
                c->mouse_left_button_down_ = true;
                Vec2 click(x, y);
                if (flags & cv::EVENT_FLAG_SHIFTKEY)
                {
                    Individual* i = c->selectIndividualFromMouseClick(click);
                    c->writeThumbnailImageToFile(i);
                }
                // else if (flags & cv::EVENT_FLAG_CTRLKEY) {}
                // else if (flags & cv::EVENT_FLAG_ALTKEY) {}
                else
                {
                    c->setLastMouseClick(click);
//                    c->wait_for_mouse_click_ = false;
                    c->setWaitForMouseClick(false);
                }
            }
            if (event == cv::EVENT_LBUTTONUP)  // Left button up.
            {
                c->mouse_left_button_down_ = false;
            }
        };
        cv::setMouseCallback(gui().getWindowName(), mouse_callback, this);
    }

    // See if click was within a Texture disk, set "worst" Individual if so.
    Individual* selectIndividualFromMouseClick(Vec2 mouse_click_position)
    {
        int p = 0;
        Individual* selected = nullptr;
        for (auto& tgm : tournament_group_.members())
        {
            Vec2 texture_center = disks_.at(p++).position;
            float distance = (texture_center - mouse_click_position).length();
            if (distance <= (textureSize() / 2)) { selected = tgm.individual; }
        }
        return selected;
    }
    
    // Write the entire "tournament" image (3 textures and background) to file.
    void writeTournamentImageToFile()
    {
        fs::path path = output_directory_this_run_;
        path /= "step_" + getStepAsString() + ".png";
        writeTournamentImageToFile(path);
    }
    
    void writeTournamentImageToFile(const std::string& image_pathname)
    {
        std::cout << "Writing tournament image to file "
                  << image_pathname << std::endl;
        cv::imwrite(image_pathname, gui().getCvMat());
    }

    // Write a "thumbnail" image with Texture and its background neighborhood.
    void writeThumbnailImageToFile(Individual* individual)
    {
        // Get index of this Individual within current TournamentGroup.
        int index = individualToTournamentIndex(individual);
        // Construct reference to thumbnail-sized square of current background.
        Vec2 size2(textureSize(), textureSize());
        Vec2 center = disks_.at(index).position;
        cv::Mat cropped_bg =
            Texture::getCvMatRect(center - size2, size2 * 2, background_image_);
        // Construct image with Texture matted over cloned (cropped) background.
        cv::Mat image = cropped_bg.clone();
        Texture* texture = GP::textureFromIndividual(individual);
        cv::Mat image_middle = Texture::getCvMatRect(size2/2, size2, image);
        texture->matteImageCacheDiskOverBG(textureSize(), image_middle);
        // Construct a name for the thumbnail image file.
        std::vector<std::string> suffixes = {"_a", "_b", "_c"};
        std::string suffix = suffixes.at(index);
        std::string step = getStepAsString();
        // Construct pathname for file.
        fs::path path = output_directory_this_run_;
        path /= ("thumbnail_" + step + suffix + ".png");
        // Write image file.
        std::cout << "Writing thumbnail image to file " << path << std::endl;
        cv::imwrite(path, image);
        // Write source code file.
        writeSourceCodeToFile(individual, "source_" + step + suffix + ".txt");
    }
    
    // Write text source code for an Individual: indented "C"-style notation.
    void writeSourceCodeToFile(Individual* individual, std::string filename)
    {
        // Construct pathname for file.
        fs::path path = output_directory_this_run_;
        path /= filename;
        std::cout << "Writing source code to file " << path << std::endl;
        // Open stream to file.
        std::ofstream output_file_stream(path);
        // Generate indented functional notation for given Individual's GpTree.
        output_file_stream << individual->tree().to_string(true);
        output_file_stream.close();
    }

    // Given an Individual, find its index within the "current" TournamentGroup.
    int individualToTournamentIndex(Individual* individual) const
    {
        int i = 0;
        int k = 0;
        for (auto& tgm : tournament_group_.members())
        {
            if (tgm.individual == individual) i = k;
            k++;
        }
        return i;
    }

    // The size of background images is adjusted by this value. It is expected
    // to be less than 1, indicating that the input photographic images are
    // generally larger than the screen resolution. Assumes only one scale is
    // needed, that the user has curated the background images to be at the
    // same scale. (If there is need for the scale to be bigger than 1 some
    // adjustments may be needed.)
    float backgroundScale() const { return background_scale_; }
    
    // Pathname of directory containing raw background image files.
    const std::string backgroundImageDirectory() const
        { return background_image_directory_; }
    
    // Collection of cv::Mat to be used as background image source material.
    const std::vector<cv::Mat>& backgroundImages() const
        { return background_images_; }
    // Add cv::Mat to collection of background images.
    void addBackgroundImage(cv::Mat& bg) { background_images_.push_back(bg); }
    
    // GUI size: drawable area in pixels.
    Vec2 guiSize() const { return gui_size_; }
    // Reference to GUI.
    GUI& gui() { return gui_; }
    const GUI& gui() const { return gui_; }

    // TODO very temp
    int textureSize() const { return 201; }

    // Get default run name from background_image_directory_.
    // (Provides consistent behavior with and without trailing "/".)
    std::string runNameDefault(const CommandLine& command_line)
    {
        fs::path path = command_line.positionalArgument(1);
        std::string fn = path.filename();
        return (fn != "") ? fn : std::string(path.parent_path().filename());
    }
    
    // A subdirectory under output_directory_ for results from this run.
    std::string runOutputDirectory()
    {
        if (!fs::exists(output_directory_))
        {
            debugPrint(output_directory_);
            assert(!"output_directory_ does not exist.");
        }
        fs::path run_output_dir = output_directory_;
        run_output_dir /= (run_name_ + "_" + date_hours_minutes());
        return run_output_dir;
    }
        
    // Returns, as a string, current Population evolution "step" number.
    std::string getStepAsString() const
    {
        return std::to_string(getPopulation()->getStepCount());
    }

    // Get/set a shared_ptr to this run's current Population.
    std::shared_ptr<Population> getPopulation() { return population_; };
    std::shared_ptr<Population> getPopulation() const { return population_; };
    void setPopulation(std::shared_ptr<Population> p) { population_ = p; };
    
    void logFunctionUsageCounts(const fs::path& out)
    {
        int step = getPopulation()->getStepCount();
        if ((step % 10) == 0)
        {
            // Preserve each named counter, but set its count to zero.
            cfu_.zeroEachCounter();
            // Count total GpFunction usage over entire Population of GpTrees.
            cfu_.count(*getPopulation());
            // Open output stream to file in append mode.
            std::ofstream outfile;
            outfile.open(out / "function_counts.txt", std::ios::app);
            if (step == 0)
            {
                std::string names = "steps,ave_tree_size,ave_fitness,";
                auto func = [&](std::string s, int c) { names += s + ","; };
                cfu_.applyToAllCounts(func);
                std::cout << names << std::endl;
                outfile << names << std::endl;
            }
            std::string counts;
            auto add_count = [&](int c){ counts += std::to_string(c) + ","; };
            add_count(step);
            add_count(getPopulation()->averageTreeSize());
            add_count(getPopulation()->averageFitness());
            cfu_.applyToAllCounts([&](std::string s, int c){ add_count(c); });
            std::cout << counts << std::endl;
            outfile << counts << std::endl;
        }
    }
    
    // Every n frames save a JPEG image of the "tournament" (whole window) and
    // append line: step number, pixel xy bounding box of all three prey.
    void writeTrainingSetData(const std::vector<Vec2>& prey_texture_positions)
    {
        int n = 10;
        if ((getPopulation()->getStepCount() % n) == 0)
        {
            // Construct path for training set directory, create if needed.
            fs::path directory = output_directory_this_run_;
            directory /= "training_set";
            fs::create_directory(directory);
            // Construct pathname image, write to file.
            fs::path image_path = directory;
            image_path /= "step_" + getStepAsString() + ".jpeg";
            cv::imwrite(image_path, gui().getCvMat());
            // Open output stream to bounding_boxes.txt file in append mode.
            std::ofstream outfile;
            outfile.open(directory / "bounding_boxes.txt", std::ios::app);
            // Construct line of text with step number and bounding boxes.
            // Note: uses OpenCV coordinates, origin in upper left.
            // Each bounding box is "(min_x min_y max_x max_y)"
            std::string bboxes = getStepAsString();
            for (auto p : prey_texture_positions)
            {
                bboxes += " (";
                bboxes += std::to_string(int(p.x())) + " ";
                bboxes += std::to_string(int(p.y())) + " ";
                bboxes += std::to_string(int(p.x() + textureSize())) + " ";
                bboxes += std::to_string(int(p.y() + textureSize())) + ")";
            }
            // Write to console log and to bounding_boxes.txt file.
            std::cout << "prey texture bounding boxes: " << bboxes << std::endl;
            outfile << bboxes << std::endl;
        }
    }
    
    // Get/set position of most recent mouse (left) click in GUI.
    Vec2 getLastMouseClick() const { return last_mouse_click_; }
    void setLastMouseClick(Vec2 mouse_pos) { last_mouse_click_ = mouse_pos; }

    // Get/set whether we are currently waiting for a mouse click in GUI.
    bool getWaitForMouseClick() const { return wait_for_mouse_click_; }
    void setWaitForMouseClick(bool wait) { wait_for_mouse_click_ = wait; }

    // Get/set whether we are currently running the step loop in run().
    bool getRunningState() const { return running_; }
    void setRunningState(bool running_state) { running_ = running_state; }


private:
    // Name of run. (Defaults to directory holding background image files.)
    const std::string run_name_;
    // Pathname of directory containing raw background image files.
    const std::string background_image_directory_;
    // Pathname of directory into which we can create a run log directory.
    const std::string output_directory_;
    // A subdirectory under output_directory_ for results from this run.
    const std::string output_directory_this_run_;
    // Collection of cv::Mat to be used as background image source material.
    std::vector<cv::Mat> background_images_;
    // The size of background images is adjusted by this value (usually < 1).
    const float background_scale_ = 1;
    // GUI size: drawable area in pixels.
    Vec2 gui_size_;
    // GUI object
    GUI gui_;
    // Seed for RandomSequence LPRS() to be used during this run
    int random_seed_ = LPRS().defaultSeed();
    // Default parameters for Population
    int individuals_ = 120;
    int subpops_ = 6;
    int max_init_tree_size_ = 100;
    int min_crossover_tree_size_ = 50;
    int max_crossover_tree_size_ = 150;
    
    // For logging GpFunction usage over evolutionary time.
    CountFunctionUsage cfu_;
    
    //-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // Note: the six variables below communicate "global" state with the mouse
    // callback handler. This is not thread safe and would need redesign for
    // multithreading. But then, since this connects directly to the GUI for an
    // interactive task, multithreading seems unlikely?
    //
    // Store position of most recent mouse (left) click in GUI.
    Vec2 last_mouse_click_;
    // True while waiting for user to select one texture on screen.
    bool wait_for_mouse_click_;
    // Collection of Disks describing layout of Textures in GUI window.
    std::vector<Disk> disks_;
    // TournamentGroup with pointers to 3 textures of most recent tournament.
    TournamentGroup tournament_group_;
    // Randomly selected rectagle of randomly selected background image.
    cv::Mat background_image_;
    // Monitor up/down status of (left) mouse button.
    bool mouse_left_button_down_ = false;
    // Points to heap-allocated Population instance during run() function.
    std::shared_ptr<Population> population_ = nullptr;
    // True during the step loop in run(). Set to false by "Q" command.
    bool running_ = false;
    //-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
};

// Experimental code for generating training set of images for learning to
// "find conspicuous disks". Generates random image files with a background
// texture and a disk of another texture matted on top. Image files maybe
// 1024 square? Textures a mix of "random" TexSyn textures and some natural
// textures from photos, as in /Users/cwr/Pictures/camouflage_backgrounds
//
// (Maybe filenames like 3485729384_0132_0981.jpg where the first number is
// just a random UID, and the second two numbers are the ground truth center
// location of the disk.)
//
// Perhaps this should be in its own file?

// command line args:
// fcd how_many
//     output_dir
//     input_photo_dir
//     seed
//     bg_scale
//     output_size
//     disk_size
//     tree_size

class GenerateTrainingSetForFindConspicuousDisks
{
    typedef fs::directory_iterator di;
    typedef fs::path pn;
public:
    // Constructor to get parameters from pre-parsed "unix style" command line.
    GenerateTrainingSetForFindConspicuousDisks(const CommandLine& cmd)
      : how_many_(cmd.positionalArgument(1, 1)),
        output_directory_(cmd.positionalArgument(2, ".")),
        input_photo_dir_(cmd.positionalArgument(3, ".")),
        random_seed_(cmd.positionalArgument(4, int(LPRS().defaultSeed()))),
        background_scale_(cmd.positionalArgument(5, float(0.5))),
        output_size_(cmd.positionalArgument(6, 1024)),
        disk_size_(cmd.positionalArgument(7, 201)),
        tree_size_(cmd.positionalArgument(8, 40))
    {
        std::cout << "GenerateTrainingSetForFindConspicuousDisks:" << std::endl;
        std::cout << "    "; debugPrint(how_many_);
        std::cout << "    "; debugPrint(output_directory_);
        std::cout << "    "; debugPrint(input_photo_dir_);
        std::cout << "    "; debugPrint(random_seed_);
        std::cout << "    "; debugPrint(background_scale_);
        std::cout << "    "; debugPrint(output_size_);
        std::cout << "    "; debugPrint(disk_size_);
        std::cout << "    "; debugPrint(tree_size_);
    }
    
    // Perform the run generating training data for "find conspicuous disks”.
    // TODO current runs forever.
    void run()
    {
        LPRS().setSeed(random_seed_);
        readAllInputPhotos();
        while (output_counter_ < how_many_) { generateOneOutputImage(); }
    }
    
    void generateOneOutputImage()
    {
        cv::Mat output = selectRandomBackgroundImage().clone();
        cv::Mat disk = selectRandomDiskImage();
        Vec2 diff = outputSize() - diskSize();
        Vec2 position(diff.x() * LPRS().frandom01(),
                      diff.y() * LPRS().frandom01());
        Vec2 center = position + diskSize() / 2;
        
        // Matte disk texture over random position in output texture.
        cv::Mat target = Texture::getCvMatRect(position, diskSize(), output);
        Texture::matteImageCacheDiskOverBG(disk, target);
        
        // Display and save the new training image.
        cv::imshow("output", output);
        // TODO needs to save to file
        std::string path = output_directory_ / outputFileName(center);
        std::cout << "Writing image " << ++output_counter_
                  << " to " << path << " ... ";
        cv::imwrite(path, output);
        std::cout << "done." <<std::endl;
    }

    // Read all input photo files as cv::Mats, adjust size, save when OK.
    void readAllInputPhotos()
    {
        Timer t("Reading background images");
        std::cout << "Reading background images:" << std::endl;
        collectPhotoPathnames(input_photo_dir_);
        std::sort(all_photo_pathnames_.begin(), all_photo_pathnames_.end());
        for (auto& pathname : all_photo_pathnames_)
        {
            // Read photo's image file into an OpenCV image.
            cv::Mat photo = cv::imread(pathname);
            // For "non small" images, adjust by background_scale_
            if (std::min(photo.rows, photo.cols) >=
                (outputSize().x() / background_scale_))
            {
                cv::resize(photo, photo,
                           cv::Size(), background_scale_, background_scale_,
                           cv::INTER_CUBIC);
            }
            // If the adjusted size is large enough
            std::cout << "    ";
            if (std::min(photo.rows, photo.cols) >= outputSize().x())
            {
                // Add to collection of background images.
                std::cout << "Adding " << pathname << std::endl;
                all_photos_.push_back(photo);
            }
            else
            {
                // Log and ignore.
                std::cout << "Ignore " << pathname << " Scaled input image "
                          << "smaller than output image size ("
                          << outputSize().x() << ")." << std::endl;
            }
        }
        std::cout << "Found " << all_photos_.size();
        std::cout << " background images." << std::endl;
        assert(!all_photos_.empty());
    }
        
    // From the given input_photo_dir, search the sub-directory tree, collecting
    // pathnames of all valid image files into all_photo_pathnames_.
    void collectPhotoPathnames(pn directory)
    {
        // For each item within the given top level directory.
        for (const auto& i : di(directory))
        {
            pn item = i;
            if (fs::is_directory(item))
            {
                // Recurse on sub-directories.
                collectPhotoPathnames(item);
            }
            else
            {
                // Save the pathnames that look like image files.
                if (cv::haveImageReader(item))
                {
                    all_photo_pathnames_.push_back(item);
                }
            }
        }
    }
    
    // Randomly select content to be the background of a generated image. This
    // may be from one of the given source photographs, or synthesized on the
    // fly as a TexSyn texture.
    cv::Mat selectRandomBackgroundImage()
    {
        // Use a photo 60% of the time, otherwise synthesize a texture.
        return (LPRS().frandom01() < 0.6 ?
                fcdSelectRandomPhoto(outputSize()) :
                fcdMakeRandomTexture(outputSize()));
    }
    
    // Randomly select content for disk on a generated image. This may be from
    // one of the given source photographs, or synthesized on the fly as a
    // TexSyn texture.
    //    cv::Mat fcdSelectRandomDiskImage()
    cv::Mat selectRandomDiskImage()
    {
        // Use a photo 30% of the time, otherwise synthesize a texture.
        return (LPRS().frandom01() < 0.3 ?
                fcdSelectRandomPhoto(diskSize()) :
                fcdMakeRandomTexture(diskSize()));
    }
    
    // Randomly select one of the given source photographs, then randomly select
    // a "size_in_pixels" rectangle within it.
    cv::Mat fcdSelectRandomPhoto(Vec2 size_in_pixels)
    {
        // Pick one of the given background photos at random.
        const cv::Mat& photo = LPRS().randomSelectElement(all_photos_);
        // How much bigger (than the output) is the background image.
        int dx = std::max(0, int(photo.cols - outputSize().x()));
        int dy = std::max(0, int(photo.rows - outputSize().y()));
        // Randomly select an offset within that size difference.
        Vec2 random_position(LPRS().randomN(dx), LPRS().randomN(dy));
        // Return a "submat" reference into the random rectangle inside "bg".
        return Texture::getCvMatRect(random_position, size_in_pixels, photo);
    }
    
    // Generate a random TexSyn texture from a random LazyPredator GpTree.
    cv::Mat fcdMakeRandomTexture(Vec2 size_in_pixels)
    {
        std::cout << "Making texture (" << size_in_pixels.x() << "x"
                  << size_in_pixels.y() << ")..." << std::flush;
        Timer t("done,");
        int max_init_tree_size = 40;
        const FunctionSet& function_set = GP::fs();
        GpTree tree;
        function_set.makeRandomTree(max_init_tree_size, tree);
        Individual individual(tree);
        Texture* texture = GP::textureFromIndividual(&individual);
        texture->rasterizeToImageCache(size_in_pixels.x(), false);
        return texture->getCvMat();
    }
    
    // Random UUID-like string plus pixel x and y of disk center.
    std::string outputFileName(Vec2 center)
    {
        return (n_letters(10, LPRS()) +
                "_" + std::to_string(int(center.x())) +
                "_" + std::to_string(int(center.y())) +
                ".jpeg");
    }

    Vec2 diskSize() const { return Vec2(disk_size_, disk_size_); }
    Vec2 outputSize() const { return Vec2(output_size_, output_size_); }

private:
    // How many training examples to generate.
    const int how_many_;
    // Pathname of directory where generated image files will be written.
    const pn output_directory_;
    // Pathname of directory where photos will be found (OK if none found).
    const pn input_photo_dir_;
    // Seed for RandomSequence LPRS() to be used during this run
    const int random_seed_ = LPRS().defaultSeed();
    // The size of background images is adjusted by this value (usually < 1).
    const float background_scale_ = 1;
    // Size (side) of square output images (in pixels).
    const float output_size_;
    // Diameter of overlaid disks (in pixels).
    const float disk_size_;
    // Maximum size of random LazyPredator GpTree for TexSyn Texture.
    const int tree_size_;
    // Collection of all image file pathnames found under input_photo_dir_.
    std::vector<pn> all_photo_pathnames_;
    // Collection of all input photos, as cv::Mats, in all_photo_pathnames_
    std::vector<cv::Mat> all_photos_;
    // Used only for logging.
    int output_counter_ = 0;
};


// PythonComms protype experiments (was EvoCamoVsStaticFCD)
//
// TODO 20211227 maybe "shared_directory_" should be a member variable.
//      Don't see a need to pass it around between all member functions.
//      Could make same argument for "step".
//
//class EvoCamoVsStaticFCD
class PythonComms
{
public:
//    // Constructor to get parameters from pre-parsed "unix style" command line.
//    // (Currently does nothing.)
//    EvoCamoVsStaticFCD(const CommandLine& cmd){}
//    PythonComms(const CommandLine& cmd){}
//    {
//        class A
//        {
//        public:
//            void foo() { std::cout << "A" << std::endl; }
//        };
//        class B : public A
//        {
//        public:
//            void foo() { std::cout << "B" << std::endl; }
//        };
//
//        A().foo();
//        B().foo();
//
//        exit(EXIT_SUCCESS);
//    }
    PythonComms(){}

    // TODO prototype for testing, just writes dummy files. Eventually would
    //      write files with newest tournament image.
    void run_test()
    {
        fs::path directory = shared_directory_;
        std::cout << "Start run in " << directory << std::endl;
        testListGDriveFiles(directory);
        auto list = listMyFiles(directory);
        if (!list.empty())
        {
            std::cout << "Unexpected files: " << vec_to_string(list) << std::endl;
        }
//        for (int step = 0; ; step++) { performStep(step, directory); }
        for (int step = 0; ; step++) { performStep(step); }
    }
    
//        // TODO this will need to take cv::Mat (or whatever) and write it to a jpeg
//        //      file with a name compatible with the Python half.
//        Vec2 performStep(int step) { return performStep(step, shared_directory_); }
//        Vec2 performStep(int step, fs::path directory)
//        {
//            std::cout << "Write file " << step << std::endl;
//    //        writeTestFile(step, directory);
//    //        auto cv_mat = cv::Mat(1, 1, CV_8UC3, cv::Scalar(0, 0, 0));
//            cv::Mat cv_mat(1, 1, CV_8UC3, cv::Scalar(0, 0, 0));
//            writeMyFile(step, directory, cv_mat);
//            deleteMyFile(step - 1, directory);
//            return waitForReply(step, directory);
//        }

    // TODO this will need to take cv::Mat (or whatever) and write it to a jpeg
    //      file with a name compatible with the Python half.
//    Vec2 performStep(int step) { return performStep(step, shared_directory_); }
    
    Vec2 performStep(int step)
    {
        return performStep(step, cv::Mat(1, 1, CV_8UC3, cv::Scalar(0, 0, 0)));
    }
    Vec2 performStep(int step, const cv::Mat& cv_mat)
    {
        return performStep(step, cv_mat, shared_directory_);
    }
    Vec2 performStep(int step, const cv::Mat& cv_mat, fs::path directory)
    {
        std::cout << "Write file " << step << std::endl;
        writeMyFile(step, directory, cv_mat);
        deleteMyFile(step - 1, directory);
        return waitForReply(step, directory);
    }
    
//    cv::Mat cv_mat(1, 1, CV_8UC3, cv::Scalar(0, 0, 0));
//
//    void writeMyFile(int step, fs::path directory)


    // From the given input_photo_dir, search the sub-directory tree, collecting
    // pathnames of all valid image files into all_photo_pathnames_.
    void testListGDriveFiles(fs::path directory)
    {
        std::cout << "Initial contents of dir:" << std::endl;
        // For each item within the given top level directory.
        for (const auto& i : fs::directory_iterator(directory))
        {
            fs::path item = i;
            std::cout << "    ";
            debugPrint(item)
        }
    }
    
    // Returns a collection of strings, each the name of one of "my files" in
    // the given directory. Names have had file's path and extension removed.
    std::vector<std::string> listMyFiles(fs::path directory)
    {
        std::vector<std::string> strings;
        for (const auto& i : fs::directory_iterator(directory))
        {
            std::string stem_string(fs::path(i).stem());
            std::string stem_prefix = stem_string.substr(0, my_prefix_.size());
            if (stem_prefix == my_prefix_) { strings.push_back(stem_string); }
        }
        return strings;
    }

//    // Mock version of writing file for given step.
//    void writeTestFile(int step, fs::path directory)
//    {
//        std::ofstream fout(makeMyPathname(step, directory));
//        // TODO 20211224 just write dummy contents.
//        fout << std::to_string(step) << std::endl;
//
//        std::cout << "wrote test file   "
//                  << makeMyPathname(step, directory) << std::endl;
//
//    }
    
    
//    writeMyFile(step, directory, cv::Mat(1, 1, CV_8UC3, Scalar(0, 0, 0)));

    // Write given image to file for given step.
    void writeMyFile(int step, fs::path directory, const cv::Mat& cv_mat)
    {
//        std::ofstream fout(makeMyPathname(step, directory));
//        // TODO 20211224 just write dummy contents.
//        fout << std::to_string(step) << std::endl;
        
        auto pathname = makeMyPathname(step, directory);
        bool image_written_to_file_ok = cv::imwrite(pathname, cv_mat);
        assert(image_written_to_file_ok);
        std::cout << "wrote test file   " << pathname << std::endl;
        
    }

    // Delete the given file, usually after having written the next one.
    void deleteMyFile(int step, fs::path directory)
    {
        fs::remove(makeMyPathname(step, directory));
    }

    // Form pathname for file of given step number from the "other" agent.
    fs::path makeOtherPathname(int step, fs::path directory)
    {
//        return directory / (other_prefix_ + std::to_string(step) + ".txt");
        return directory / (other_prefix_ + std::to_string(step) + other_suffix_);
    }
    
    // Form pathname for file of given step number from "this" agent.
    fs::path makeMyPathname(int step, fs::path directory)
    {
//        return directory / (my_prefix_ + std::to_string(step) + ".txt");
        return directory / (my_prefix_ + std::to_string(step) + my_suffix_);
    }

    // Wait until other agent's file for given step appears.
//    void waitForReply(int step, fs::path directory)
    Vec2 waitForReply(int step, fs::path directory)
    {
        Timer t("Elapsed time");
        std::cout << "start waiting for " << makeOtherPathname(step, directory) << std::endl;
        while (!isFilePresent(makeOtherPathname(step, directory)))
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));  // wait 2 sec
        }
        std::cout << "done waiting for  " << makeOtherPathname(step, directory) << std::endl;
        
        // TODO 20211230 -- read XY out of file
        
        
        
        
//                std::ofstream fout(makeMyPathname(step, directory));
//                // TODO 20211224 just write dummy contents.
//                fout << std::to_string(step) << std::endl;
        
        std::ifstream input_file(makeOtherPathname(step, directory));
        
//        if (!input_file.is_open()) {
//            cerr << "Could not open the file - '"
//            << filename << "'" << endl;
//            return EXIT_FAILURE;
//        }

        float x, y;
        input_file >> x;
        input_file >> y;
        input_file.close();
        Vec2 position(x, y);
        debugPrint(position);
        return position;
    }

    // Like fs::exists() but for unknown reasons, that does not
    // seem to work for newly created files on G Drive.
    //
    // TODO Why? This version works on G Drive, but it seems simply
    //      calling fs::exists() should be enough.
    //
    bool isFilePresent(fs::path file)
    {
        bool result = false;
        fs::path directory = file.parent_path();
        std::string filename = file.filename();
        for (const auto& i : fs::directory_iterator(directory))
        {
            std::string dir_item_name(fs::path(i).filename());
            if (filename == dir_item_name) { result = true; }
        }
        return result;
    }

private:
    // Shared "communication" directory on Drive.
    std::string shared_directory_ =
        "/Volumes/GoogleDrive/My Drive/PredatorEye/evo_camo_vs_static_fcd/";
    std::string my_prefix_ = "camo_";
    std::string other_prefix_ = "find_";
    std::string my_suffix_ = ".jpeg";
    std::string other_suffix_ = ".txt";
};


// TODO 20211229 — make a new class derived from EvoCamoGame
//     The parts that need to be changed
//        waitForUserInput() — needs to be conditionalized or overridden
//        setLastMouseClick() — need to call with result from Colab
//     Relevant to changes:
//        tournamentFunction()
//        setMouseCallbackForTournamentFunction() —
//            can stay the same but functionality need to be replaced
//

class EvoCamoVsStaticFCD : public EvoCamoGame
{
public:
    EvoCamoVsStaticFCD(const CommandLine& cmd) : EvoCamoGame(cmd) {}
    

    // Ad hoc idle loop, waiting for user input. Exits on left mouse click, the
    // user's selection of the "worst" camouflage. This also "listens" for and
    // executes single character commands: "t" and "Q".
    // (20211230 make virtual so can be overridden, eg by EvoCamoVsStaticFCD)
    void waitForUserInput() override
    {
        int step = getPopulation()->getStepCount();
        Vec2 prediction = getComms().performStep(step, gui().getCvMat());
        setLastMouseClick(prediction);
        
        gui().drawCircle(textureSize() * 0.55, prediction * 1024, Color(1));
        gui().refresh();
        
        // TODO very temp
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    PythonComms& getComms() { return comms_; }
private:
    PythonComms comms_;
};
