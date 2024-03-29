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
// The code in this file became the paper:
//
//     Craig Reynolds. 2023. Coevolution of Camouflage. 2023 Artificial Life
//     Conference. MIT Press. https://doi.org/10.1162/isal_a_00583
//         Preprint with supplemental materials and high resolution images:
//         https://arxiv.org/abs/2304.11793
//
// Prototyping inside TexSyn, maybe should be its own library/git repository.
//
//------------------------------------------------------------------------------

#pragma once
#include "GP.h"
#include "Texture.h"
#include "Utilities.h"
#include "RandomSequence.h"

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
        max_steps_(cmd.positionalArgument(9, 12000)),
        max_init_tree_size_(cmd.positionalArgument(10, 100)),
        min_crossover_tree_size_
            (cmd.positionalArgument(11, max_init_tree_size_ * 0.5f)),
        max_crossover_tree_size_
            (cmd.positionalArgument(12, max_init_tree_size_ * 1.5f))
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
            std::cout << "    max_steps (defaults to 12000)" << std::endl;
            std::cout << "    max_init_tree_size (defaults to 100)"<< std::endl;
            std::cout << "    min_crossover_tree_size "
                         "(default max_init_tree_size_ * 0.5)" << std::endl;
            std::cout << "    max_crossover_tree_size "
                         "(default max_init_tree_size_ * 1.5)" << std::endl;
            std::cout << std::endl;
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
            debugPrint(max_steps_);
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
                           cv::INTER_AREA);
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
        std::cout << std::endl;
        std::cout <<"Width of GUI:  "<<guiSize().x()<<", of scaled bg images: "
            << s_min_x << ", ratio: " << s_min_x / guiSize().x() << std::endl;
        std::cout <<"Height of GUI: "<<guiSize().y()<<", of scaled bg images: "
            << s_min_y << ", ratio: " << s_min_y / guiSize().y() << std::endl;
        std::cout << std::endl;
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
        // Cannot run without background image directory.
        if (backgroundImageDirectory().empty()) return;
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
        fs::path out = outputDirectoryThisRun();
        std::cout << "Create output directory for this run: ";
        std::cout << out << std::endl;
        fs::create_directory(out);
        // Init GUI window.
        gui().setWindowName(run_name_);
        gui().refresh();
        // Loop of interactive evolution steps, until "Q" command or forced exit.
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
            setRunningState(getMaxSteps() >= getStepCount());
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
        // Initialize "global variables" used by mouse callback handler.
        tournament_group_ = tg;
        background_image_ = selectRandomBackgroundForWindow();
        // Generate and store random non-overlapping prey disks in gui window.
        generatePreyPlacement();
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
        // Mark returned TournamentGroup as invalid if predator failed to locate
        // a prey. That is, if either: the user's mouse click or the xy position
        // returned from the "predator vision" neural net--is not inside any of
        // the three disks.
        if (worst == nullptr)
        {
            tg.setValid(false);
            tournament_group_ = tg;
            invalidTournamentGroupHook();
            incrementPredatorFails();
            std::cout << "    Predator fooled: no prey selected." << std::endl;
        }
        // Clear GUI, return updated TournamentGroup.
        gui().clear();
        gui().refresh();
        return tg;
    }

    // For customizations by derived classed (below).
    virtual void invalidTournamentGroupHook() {}
    
    // TODO 20230929 Keep or move elsewhere? This was added to allow calling
    // it from here but implementing it in derived classes. But then I added
    // invalidTournamentGroupHook() as a cleaner way to do that. saveThisStep()
    // is no longer used in EvoCamoGame.
    virtual bool saveThisStep() { return false; }

    // Generate and store random non-overlapping prey disks in gui window.
    virtual void generatePreyPlacement()
    {
        // Restrict Texture disks to be completely inside a rectangle inset
        // from the window edge a Texture's radius. Rectangle defined by two
        // diagonally opposite corners.
        float radius = textureSize() / 2;
        Vec2 rect_min = Vec2(radius + 1, radius + 1);
        Vec2 rect_max = guiSize() - rect_min;
        // Find non-overlapping positions for the Textures in TournamentGroup.
        float margin = radius;
        // TODO dummy function, should be cleaned up (removed).
        auto overlap_viz = [&](const std::vector<Disk>& disks)
            { /*testdraw(tg,disks,rect_min,rect_max,textureSize()+margin);*/ };
        setPreyDisks(Disk::randomNonOverlappingDisksInRectangle(3, radius,
                     radius, margin, rect_min, rect_max, LPRS(), overlap_viz));
    }

    // Count "invalid tournaments" -- aka "predator fails"
    int getPredatorFails() const { return predator_fails_; }
    void incrementPredatorFails() { predator_fails_++; }

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
            Vec2 position = getPreyCenter(p++) - center_to_ul;
            prey_texture_positions.push_back(position);
            cv::Mat target = gui().getCvMatRect(position, Vec2(size, size));
            texture->matteImageCacheDiskOverBG(size, target);
        }
        // TODO 20211010 turn this off. If needed later add global enable flag.
        // writeTrainingSetData(prey_texture_positions);
    }

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
                else
                {
                    c->setLastMouseClick(click);
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
            Vec2 offset = getPreyCenter(p++) - mouse_click_position;
            float distance = offset.length();
            if (distance <= (textureSize() / 2)) { selected = tgm.individual; }
        }
        return selected;
    }

    // Write current "tournament" image (3 textures and background) to file.
    void writeTournamentImageToFile()
    {
        fs::path path = outputDirectoryThisRun();
        path /= "step_" + getStepAsString() + ".png";
        writeTournamentImageToFile(path, gui().getCvMat());
    }
    
    // Write given "tournament" image to given pathname. Also note on log.
    void writeTournamentImageToFile(const fs::path& pathname,
                                    const cv::Mat& image)
    {
        std::cout << "Writing tournament image to file ";
        std::cout << pathname.string() << std::endl;
        cv::imwrite(pathname, image);
    }

    // Write a "thumbnail" image with Texture and its background neighborhood.
    void writeThumbnailImageToFile(Individual* individual)
    {
        // Get index of this Individual within current TournamentGroup.
        int index = individualToTournamentIndex(individual);
        // Construct reference to thumbnail-sized square of current background.
        Vec2 size2(textureSize(), textureSize());
        Vec2 center = getPreyCenter(index);
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
        fs::path path = outputDirectoryThisRun();
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
        fs::path path = outputDirectoryThisRun();
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
    int texture_size_ = 201;
    // TODO 20211231 for backward compatibility / testing
    int textureSize() const { return getTextureSize(); }
    int getTextureSize() const { return texture_size_; }
    void setTextureSize(int size) { texture_size_ = size; }

    // Get default run name from background_image_directory_.
    // (Provides consistent behavior with and without trailing "/".)
    // ((Provides "no_name" as ultimate fall back for empty command line.))
    std::string runNameDefault(const CommandLine& command_line)
    {
        fs::path path = command_line.positionalArgument(1);
        std::string fn = path.filename();
        if (fn.empty()) { fn = std::string(path.parent_path().filename()); }
        if (fn.empty()) { fn = "no_name"; }
        return fn;
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20221013 actually write log to file.
    
//    // A subdirectory under output_directory_ for results from this run.
//    std::string runOutputDirectory()
//    {
//        if (!fs::exists(output_directory_))
//        {
//            debugPrint(output_directory_);
//            assert(!"output_directory_ does not exist.");
//        }
//        fs::path run_output_dir = output_directory_;
//        run_output_dir /= (run_name_ + "_" + date_hours_minutes());
//        return run_output_dir;
//    }

    // A subdirectory under output_directory_ for results from this run.
    std::string runOutputDirectory()
    {
        if (!fs::exists(output_directory_))
        {
            debugPrint(output_directory_);
            assert(!"output_directory_ does not exist.");
        }
        fs::path run_output_dir = output_directory_;
        
        run_id_ = run_name_ + "_" + date_hours_minutes();
//        run_output_dir /= (run_name_ + "_" + date_hours_minutes());
        run_output_dir /= run_id_;

        return run_output_dir;
    }
    
    std::string getRunID() const { return run_id_; }

    std::string run_id_;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Returns, as a string, current Population evolution "step" number.
    std::string getStepAsString() const
    {
        return getStepAsString(0);
    }
    
    // Returns, as a string, current Population "step" number plus signed offset.
    std::string getStepAsString(int offset) const
    {
        return std::to_string(offset + getStepCount());
    }

    // Get/set a shared_ptr to this run's current Population.
    std::shared_ptr<Population> getPopulation() { return population_; };
    std::shared_ptr<Population> getPopulation() const { return population_; };
    void setPopulation(std::shared_ptr<Population> p) { population_ = p; };
    
    void logFunctionUsageCounts(const fs::path& out)
    {
        int step = getStepCount();
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
        if ((getStepCount() % n) == 0)
        {
            // Construct path for training set directory, create if needed.
            fs::path directory = outputDirectoryThisRun();
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

    // Accessor for command line argument used by derived classes.
    std::string outputDirectoryThisRun() const
        { return output_directory_this_run_; }
    
    // Get/set collection of prey Disks within GUI window.
    std::vector<Disk> getPreyDisks() const { return disks_; }
    void setPreyDisks(const std::vector<Disk>& disks) { disks_ = disks; }
    // Get center position of i-th prey disk.
    Vec2 getPreyCenter(int i) const { return disks_.at(i).position; }
    
    // Read-only access to most recently saved (copied) TournamentGroup().
    const TournamentGroup& getTournamentGroup() const {return tournament_group_;}

    // Current simulation step, held by Population object.
    int getStepCount() const { return getPopulation()->getStepCount(); }

    // Exit simulation after this many steps.
    int getMaxSteps() const { return max_steps_; }

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
    // Seed for RandomSequence LPRS() to be used during this run
    int random_seed_ = LPRS().defaultSeed();
    // GUI size: drawable area in pixels.
    Vec2 gui_size_;
    // GUI object
    GUI gui_;
    // Default parameters for Population
    int individuals_ = 120;
    int subpops_ = 6;
    int max_init_tree_size_ = 100;
    int min_crossover_tree_size_ = 50;
    int max_crossover_tree_size_ = 150;
    
    // For logging GpFunction usage over evolutionary time.
    CountFunctionUsage cfu_;
    
    // Count "invalid tournaments" -- aka "predator fails"
    int predator_fails_ = 0;

    // Exit simulation after this many steps.
    int max_steps_ = 12000;

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

// TODO this should be in its own file.

// Experimental code for generating training set of images for learning to
// "find conspicuous disks". Generates random image files with a background
// texture and a disk of another texture matted on top. Image files maybe
// 1024 square? Textures a mix of "random" TexSyn textures and some natural
// textures from photos, as in /Users/cwr/Pictures/camouflage_backgrounds
//
// (Maybe filenames like 3485729384_0132_0981.jpg where the first number is
// just a random UID, and the second two numbers are the ground truth center
// location of the disk.)

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
    
    // TODO 20220127 adjust bg scale by output_size_
    const int dos_ = 1024; // Default output size (to adjust background_scale_).
public:
    // Constructor to get parameters from pre-parsed "unix style" command line.
    GenerateTrainingSetForFindConspicuousDisks(const CommandLine& cmd)
      : how_many_(cmd.positionalArgument(1, 1)),
        output_directory_(cmd.positionalArgument(2, ".")),
        input_photo_dir_(cmd.positionalArgument(3, ".")),
        random_seed_(cmd.positionalArgument(4, int(LPRS().defaultSeed()))),
        background_scale_(cmd.positionalArgument(5, float(0.5)) *
                          (cmd.positionalArgument(6, dos_) / float(dos_))),
        output_size_(cmd.positionalArgument(6, dos_)),
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
//        while (output_counter_ < how_many_) { generateOneOutputImage(); }
        while (outputCounter() < how_many_) { generateOneOutputImage(); }
    }
    
    virtual void generateOneOutputImage()
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
                
        writeOneOutputImage(output, center);
    }
    
    void writeOneOutputImage(const cv::Mat& output, Vec2 center)
    {
        // Display and save the new training image.
        cv::imshow("output", output);
        // TODO needs to save to file
        std::string path = output_directory_ / outputFileName(center);
//        std::cout << "Writing image " << ++output_counter_
//                  << " to " << path << " ... ";
        incrementOutputCounter();
        std::cout << "Writing image " << outputCounter()
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
            // For "non small" images, adjust by background_scale_.
            // (The buggy special case is for the fungus photos. Revisit?)
            int min_dim = std::min(photo.rows, photo.cols);
            bool big_enough = min_dim >= (outputSize().x() / background_scale_);
            float scale = background_scale_ * (big_enough ? 1 : 2);
            cv::resize(photo, photo, cv::Size(), scale, scale, cv::INTER_AREA);
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
    // Changed to loop until the texture is not boringly uniform. (20220127)
    cv::Mat fcdMakeRandomTexture(Vec2 size_in_pixels)
    {
        while (true)
        {
            std::cout << "Making texture (" << size_in_pixels.x() << "x"
                      << size_in_pixels.y() << ")..." << std::flush;
            Timer t("done,");
            int max_init_tree_size = tree_size_;
            const FunctionSet& function_set = GP::fs();
            GpTree tree;
            function_set.makeRandomTree(max_init_tree_size, tree);
            Individual individual(tree);
            Texture* texture = GP::textureFromIndividual(&individual);
            texture->rasterizeToImageCache(size_in_pixels.x(), false);
            cv::Mat cv_mat = texture->getCvMat();
            if (Texture::matUniformity(cv_mat) < 0.9) { return cv_mat; }
            std::cout << "reject uniform texture...";
        }
    }
    
    // Random UUID-like string plus pixel x and y of disk center.
    std::string outputFileName(Vec2 center)
    {
        return (n_letters(10, LPRS()) +
                "_" + std::to_string(int(center.x())) +
                "_" + std::to_string(int(center.y())) +
                getOutputFilenameExtension());
    }
    
    // Get/set image file format for output, specified by pathname extension.
    std::string getOutputFilenameExtension() const
    {
        return output_filename_extension_;
    }
    void setOutputFilenameExtension(std::string extension)
    {
        output_filename_extension_ = extension;
    }

    Vec2 diskSize() const { return Vec2(disk_size_, disk_size_); }
    Vec2 outputSize() const { return Vec2(output_size_, output_size_); }
    int treeSize() const { return tree_size_; }
    
    // TODO 20220302 add accessor to make visibe from derived classes.
    int outputCounter() const { return output_counter_; }
    void incrementOutputCounter() { output_counter_++; }

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
    // (OK, as of 20220302, also used for cycling through types.)
    int output_counter_ = 0;
    // Type of image file format for output, specified by pathname extension.
    std::string output_filename_extension_ = ".jpeg";
};

class GenerateTrainingSetForFind3Disks :
    public GenerateTrainingSetForFindConspicuousDisks
{
public:
    // Use constructor from base class (pre-parsed "unix style" command line).
    GenerateTrainingSetForFind3Disks(const CommandLine& cmd) :
        GenerateTrainingSetForFindConspicuousDisks(cmd)
    {
        setOutputFilenameExtension(".png");
    }

    // Select a random background (as in the base class) and then overlay THREE
    // disks (each selected as before). Because these are being rendered at very
    // low 128x128 resolution, we draw the disks as "soft edged spots" with the
    // outer 15% of radius used to blend into the background texture. Training
    // examples are LABELED with the center position of disk 0, the one rendered
    // fully opaque (except for edges). The other two disks are drawn in a way
    // intended to reduce their "conspicuousness". Disk 1 is drawn with at least
    // 50% transparency. Transparency of disk 2 is modulated by confetti noise.
    void generateOneOutputImage() override
    {
        cv::Mat output = selectRandomBackgroundImage().clone();
        // Render style for the three disks:
        //  0: opaque (its center is the deep learning "label" for this example)
        //  1: 50% transparent (distracter).
        //  2: confettii transparency (distracter).
        std::vector<Vec2> centers = find3RandomDiskCenters();
        for (int i = 0; i < centers.size(); i++)
        {
            cv::Mat disk = selectRandomDiskImage();
            Vec2 position = centers.at(i) - diskSize() / 2;
            // Matte disk texture over random position in output texture.
            cv::Mat target = Texture::getCvMatRect(position, diskSize(), output);
            float radius = diskSize().x() * 0.5;
            matteSoftDiskOverBG(radius * 0.85,
                                radius,
                                (i == 1 ? 0.5 : 1),  // matte_opacity
                                i == 2,              // matte_randomize
                                disk,
                                target);
        }
        // Write texture to image file, first centerpoint encoded in filename.
        writeOneOutputImage(output, centers.at(0));
    }

    // Used to blend each of the three disks into the background image. In the
    // base class this is done with Texture::matteImageCacheDiskOverBG(). This
    // version draws disks as "soft edged spots" and allows separate controls
    // for overall opacity and multiplying by midrange confetti noise.
    void matteSoftDiskOverBG(float inner_radius,
                             float outer_radius,
                             float matte_opacity,
                             bool matte_randomize,
                             const cv::Mat& disk,
                             cv::Mat& bg)
    {
        // Expect two square Mats, of same size, and type CV_8UC3.
        assert(bg.rows == bg.cols);
        assert(bg.rows == disk.rows);
        assert(disk.rows == disk.cols);
        assert(bg.type() == CV_8UC3);
        assert(disk.type() == CV_8UC3);
        // Diameter of disk.
        int diameter = disk.rows;
        // Centerpoint of disk (as float, independent of pixel boundaries).
        Vec2 cp(diameter / 2.0, diameter / 2.0);
        // Loop over all pixel coordinates (xi,yi) with center of pixel (xf,yf).
        for (int xi = 0; xi < diameter; xi++)
        {
            for (int yi = 0; yi < diameter; yi++)
            {
                // Position of pixel center.
                Vec2 pos(xi + 0.5, yi + 0.5);
                float spot = spot_utility(pos, cp, inner_radius, outer_radius);
                // When inside the spot (when spot opacity is not zero).
                if (spot > 0)
                {
                    // Adjust by "matte_opacity" and "matte_randomize" params.
                    float matte = spot * matte_opacity;
                    if (matte_randomize) { matte *= LPRS().random2(0.3f, 0.7f);}
                    // Blend disk and background colors, according to "matte".
                    Color color = interpolate(matte,
                                              Texture::matPixelRead(bg, pos),
                                              Texture::matPixelRead(disk, pos));
                    Texture::matPixelWrite(bg, pos, color);
                }
            }
        }
    }

    // Find 3 non-overlapping disk positions inside image.
    // Uses simple greedy approach, assuming that the 3 disks will easily fit.
    // (Rewritten from Find_3_Disks.ipynb.)
    std::vector<Vec2> find3RandomDiskCenters()
    {
        auto random_center = [&]()
        {
            Vec2 excess = outputSize() - (diskSize() * 1.5);
            return (diskSize() * 0.75 +
                    Vec2(LPRS().frandom01() * excess.x(),
                         LPRS().frandom01() * excess.y()));
        };
        std::vector<Vec2> centers = {random_center()};
        float min_dist = diskSize().x() * 1.5;
        while (centers.size() < 3)
        {
            Vec2 c = random_center();
            bool all_ok = true;
            for (auto o : centers)
            {
                if ((c - o).length() < min_dist) { all_ok = false; }
            }
            if (all_ok) { centers.push_back(c); }
        }
        return centers;
    }
};

// FCD1: original [date?] with one prey per example, 1kx1k!!
// FCD2: F3D-simple [date?] 3 prey but they and background are Uniform
// FCD3: F3D-complex [date?] 3 prey, each (and background) are photo or TexSyn,
//       1 prey corresponds to label, other two distractors are less conspicuous
// FCD4: like FCD3 but distractors are less conspicuous, but not as much less,
//       switched to using PNG files.
// FCD5: mixture of three types of prey
//
// Make a new "FCD5" dataset which has three types of examples:
//     1: like FCD: one random textured prey on a random textured background
//     2: like F3D-complex: 3 prey, the labeled one is full strength, another is
//        half blended into the background, another is noise-matted
//     3: new one where 1 prey is used, but blended in at full and two partial
//        opacities, perhaps randomized for each example
//
// TODO this replaces the generateOneOutputImage() method in two classes:
// GenerateTrainingSetForFindConspicuousDisks, GenerateTrainingSetForFind3Disks.
// They could be collapsed together once I decide what kind of dataset is best.

class GenerateDatasetFCD5 : public GenerateTrainingSetForFind3Disks
{
public:
    GenerateDatasetFCD5(const CommandLine& cmd)
      : GenerateTrainingSetForFind3Disks(cmd) {}

    void generateOneOutputImage() override
    {
        int option = outputCounter() % 3;
        if (option == 0) { singlePrey(); }
        if (option == 1) { threeUniquePreyTwoMuted(); }
        if (option == 2) { threeCopiedPreyTwoMuted(); }
    }
    
    void singlePrey() { generaterPrototype(); }
    void threeUniquePreyTwoMuted() { tuptmPrototype(); }
    void threeCopiedPreyTwoMuted() { tcptmPrototype(); }
    
    // TODO some kind of superset of all options?
    void generaterPrototype()
    {
        cv::Mat output = selectRandomBackgroundImage().clone();
        cv::Mat disk = selectRandomDiskImage();
        Vec2 center = find3RandomDiskCenters().at(0);
        Vec2 ul_corner = center - diskSize() / 2;
        // Matte disk texture over random position in output texture.
        cv::Mat target = Texture::getCvMatRect(ul_corner, diskSize(), output);
        Texture::matteImageCacheDiskOverBG(disk, target);
        writeOneOutputImage(output, center);
    }
    
    
    // threeUniquePreyTwoMuted case
    void tuptmPrototype()
    {
        cv::Mat output = selectRandomBackgroundImage().clone();
        // Render style for the three disks:
        //  0: opaque (its center is the deep learning "label" for this example)
        //  1: 50% transparent (distracter).
        //  2: confettii transparency (distracter).
        std::vector<Vec2> centers = find3RandomDiskCenters();
        for (int i = 0; i < centers.size(); i++)
        {
            cv::Mat disk = selectRandomDiskImage();
            Vec2 position = centers.at(i) - diskSize() / 2;
            // Matte disk texture over random position in output texture.
            cv::Mat target = Texture::getCvMatRect(position, diskSize(), output);
            float radius = diskSize().x() * 0.5;
            matteSoftDiskOverBG(radius * 0.85,
                                radius,
                                (i == 1 ? 0.5 : 1),  // matte_opacity
                                i == 2,              // matte_randomize
                                disk,
                                target);
        }
        // Write texture to image file, first centerpoint encoded in filename.
        writeOneOutputImage(output, centers.at(0));
    }
    
    // threeCopiedPreyTwoMuted case
    void tcptmPrototype()
    {
        cv::Mat output = selectRandomBackgroundImage().clone();
        cv::Mat disk = selectRandomDiskImage();
        std::vector<Vec2> centers = find3RandomDiskCenters();
        for (int i = 0; i < centers.size(); i++)
        {
            Vec2 position = centers.at(i) - diskSize() / 2;
            // Matte disk texture over random position in output texture.
            cv::Mat target = Texture::getCvMatRect(position, diskSize(), output);
            float radius = diskSize().x() * 0.5;
            matteSoftDiskOverBG(radius * 0.85,
                                radius,
                                1 - (i * 0.25),  // matte_opacity (1, .75, .5)
                                false,           // matte_randomize
                                disk,
                                target);
        }
        // Write texture to image file, first centerpoint encoded in filename.
        writeOneOutputImage(output, centers.at(0));
    }
};

// PythonComms helper class. Mediates communication between local c++ code on my
// laptop with Python/Keras/TensorFlow code on Colab cloud.
class PythonComms
{
public:
    PythonComms(){}

    // Perform one step of communication with the Python side.
    Vec2 performStep(int step, const cv::Mat& cv_mat)
    {
        writeMyFile(step, resizeImage(cv_mat));
        deleteMyFile(step - 1);
        return waitForReply(step);
    }

    // Write given image to file for given step.
    void writeMyFile(int step, const cv::Mat& cv_mat)
    {
        writeFile(makeMyPathname(step), cv_mat);
    }

    // Write given image to given file pathname.
    void writeFile(fs::path pathname, const cv::Mat& cv_mat)
    {
        verifyCommsDirectoryReachable();
        bool image_file_written_ok = cv::imwrite(pathname, cv_mat);
        assert(image_file_written_ok);
    }

    // Delete file for given step, presumably after having written the next one.
    void deleteMyFile(int step)
    {
        verifyCommsDirectoryReachable();
        fs::remove(makeMyPathname(step));
    }

    // Form pathname for file of given step number from the "other" agent.
    fs::path makeOtherPathname(int step)
    {
        return makePathname(step, other_prefix_, other_suffix_);
    }

    // Form pathname for file of given step number from "this" agent.
    fs::path makeMyPathname(int step)
    {
        return makePathname(step, my_prefix_, my_suffix_);
    }
    
    // Generic make pathname helper.
    fs::path makePathname(int step, std::string prefix, std::string suffix)
    {
        return sharedDirectory() / (prefix + std::to_string(step) + suffix);
    }

    
    // Wait until Python side's response file for given step appears. Parse that
    // file into 2 float values, an x and y of the prediction in image-relative
    // coordinates (each on [0, 1]), return as Vec2.
    Vec2 waitForReply(int step)
    {
        auto my_pathname = makeMyPathname(step);
        auto others_pathname = makeOtherPathname(step);
        std::string status_line = ("    Wrote " +
                                   my_pathname.filename().string() +
                                   ", waiting for " +
                                   others_pathname.filename().string());
        std::cout << status_line << std::flush;
        // Wait until response file appears.
        int seconds_waiting = 0;
        std::string temp_part;
        auto erase_temp_text = [](std::string s)
            { for (int i = 0; i < s.size(); i++) { std::cout << "\b \b"; }  };
        key_typed_1_ = false;
        key_typed_2_ = false;
        std::string extra_temp_msg;
        while (!isFilePresent(others_pathname))
        {
            // Wait 1 second (4 * 1/4 second (250/1000)). Use cv::waitKey
            // so that the OpenCV window responds to select/hide commands.
            for (int i = 0; i < 4; i++)
            {
                int key = cv::waitKey(250);
                std::string queued = ", queued save image for step ";
                switch (key) {
                    case -1:
                        break;
                    case '1':
                        key_typed_1_ = true;
                        extra_temp_msg = queued + std::to_string(step);
                        break;

                    case '2':
                        key_typed_2_ = true;
                        extra_temp_msg = queued + std::to_string(step - 1);
                        break;
                    default:
                        extra_temp_msg = ", unknown key command: ";
                        extra_temp_msg += char(key);
                }
                
            }
            seconds_waiting++;
            int count_down = previous_cycle_seconds_ - seconds_waiting;
            erase_temp_text(temp_part);
            temp_part = (extra_temp_msg + ", expected in: " +
                         std::to_string(count_down) + "...");
            std::cout << temp_part << std::flush;
            if (seconds_waiting % 100 == 0) writePingFile(seconds_waiting, step);
        }
        // Blend this cycle time into accumlator as estimate for next cycle.
        previous_cycle_seconds_ = interpolate(0.3,
                                              float(seconds_waiting),
                                              previous_cycle_seconds_);
        erase_temp_text(temp_part);
        std::cout << ", waited " << seconds_waiting << " seconds." << std::endl;
        // Read and store the contents of the predator's response file.
        readAndSaveResponsesFile(others_pathname);
        return allResponses().front();
    }
    
    // Returns (a copy of) “all predator responses” from the most recent call to
    // PythonComms::waitForReply() this is a vector of Vec2 representing the xy
    // position (image normalized coordinates) each representing the output
    // (prediction / estimate) of a predator’s CNN. Before 20220831 there was
    // only one of these, now there can be any number, but typically three. They
    // are sorted according to a quality metric so the first one is “best.”
    std::vector<Vec2> allResponses() const { return predator_responses_; }
    
    // Read the response file (eg "find_2731.txt") from the Python PredatorEye
    // side, store it in "predator_responses_". Since switching to local mode
    // from "Rube Goldberg mode", I have seen occasional crashes (segmentation
    // fault) at the last line of waitForReply(). (See logs in Notes app for run
    // "oxalis_sprouts_20221003_2021" and "michaels_gravel_20221001_1748".) My
    // theory is that while it waits until the file exists, perhaps data is
    // still being written to it? If so, this could leave predator_responses_
    // empty, and so allResponses().front() is undefined behavior. On 20221005
    // I changed this to loop until it has read the expected number of responses
    // from the file. (Maybe Rube Goldberg hid this bug with long sync delays.)
    void readAndSaveResponsesFile(fs::path others_pathname)
    {
        predator_responses_ =
            readResponseFile(others_pathname, expected_response_count_);
    }

    // Read a "response file" of xy floats from PredatorEye from the given
    // pathname. Reread until the expected number of points is found.
    // (TODO 20230322 this is to deal with reading partially written files. I
    //       now prefer the aprpoach used in EvoCamoVsLppSqm where the file is
    //       written to a temporary pathname, then renamed when completed.)
    std::vector<Vec2> readResponseFile(fs::path pathname, int expected_points)
    {
        std::vector<Vec2> xy_points;
        while (true)
        {
            float x, y;
            std::ifstream input_file(pathname);
            while ((input_file >> x) && (input_file >> y))
            {
                xy_points.push_back(Vec2(x, y));
            }
            input_file.close();
            if (xy_points.size() >= expected_points) {break;}
            std::cout << "Reread: bad format for " << pathname;
            std::cout << std::endl;
        }
        return xy_points;
    }
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20221012 build out expected_response_count_ api
    // Get/set the expected number of predator responses (prediction xy).
    int getExpectedResponseCount() const { return expected_response_count_; }
    void setExpectedResponseCount(int c) { expected_response_count_ = c; }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Used to ping the comms directory when it seems hung.
    void writePingFile(int count, int step)
    {
        verifyCommsDirectoryReachable();
        auto pathname = makePathname(step, "ping_earth_", ".txt");
        std::ofstream output_file(pathname);
        output_file << count;
        std::cout << "Ping comms: " << count << " " << pathname << std::endl;
    }

    // Like fs::exists() but will wait, and complain to the log, if the "comms"
    // directory on Google Drive is inaccessible.
    bool isFilePresent(fs::path file)
    {
        verifyCommsDirectoryReachable();
        std::error_code error_code_to_prevent_exceptions;
        return fs::exists(file, error_code_to_prevent_exceptions);
    }

    // Just wait in retry loop if shared "comms" directory become unreachable.
    // Probably will return shortly, better to wait than signal a file error.
    void verifyCommsDirectoryReachable()
    {
        int seconds = 0;
        auto sd = sharedDirectory();
        while (!fs::exists(sd))
        {
            std::cout << "Shared “comms” directory " << sd.string();
            std::cout << " has been inaccessible for " << seconds;
            std::cout << " seconds" << std::endl;
            cv::waitKey(1000);
            seconds++;
        }
    }

    // Force image to be size expected by Python DNN side, allowing c++ TexSyn
    // side to run at higher resolution for visual quality.
    cv::Mat resizeImage(const cv::Mat& cv_mat)
    {
        cv::Mat resized;
        cv::Size expected(expectedImageSize(), expectedImageSize());
        cv::resize(cv_mat, resized, expected, 0, 0, cv::INTER_AREA);
        return resized;
    }
    
    // Size of square image expected by DNN side.
    int expectedImageSize() const { return expected_image_size_; }

    // Pathname of shared "comms" directory (on Google Drive thus far)
    fs::path sharedDirectory() const { return shared_directory_; }
    
    // Was a given key typed during the previous waitForReply()?
    bool keyTyped1() const { return key_typed_1_; }
    bool keyTyped2() const { return key_typed_2_; }

private:
    // Shared "communication" directory for passing per-step files between
    // TexSyn and PredatorEye. (Conditionalize for "Rube Goldberg" mode in
    // which the comms dir is on Google Drive mounted via "Google Drive for
    // Desktop". No longer used.)
    bool rube_goldberg_mode_ = false;
    fs::path local_comms_ = "/Users/cwr/camo_data/comms/";
    fs::path g_drive_comms_ = ("/Users/cwr/Library/CloudStorage/"
                               "GoogleDrive-craig.w.reynolds@gmail.com/"
                               "My Drive/PredatorEye/evo_camo_vs_static_fcd");
    fs::path shared_directory_ = (rube_goldberg_mode_ ?
                                  g_drive_comms_ :
                                  local_comms_);

    std::string my_prefix_ = "camo_";
    std::string other_prefix_ = "find_";
    std::string my_suffix_ = ".png";
    std::string other_suffix_ = ".txt";
    float previous_cycle_seconds_ = 10;  // Initialize to typical value.
    int expected_image_size_ = 128;
    bool key_typed_1_ = false;
    bool key_typed_2_ = false;
    // All predator responses from most recent PythonComms::waitForReply() call.
    std::vector<Vec2> predator_responses_;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20221012 build out expected_response_count_ api
    // TODO 20221005 should default be 1, set to 3 by eg EvoCamoVsLearnPredPop?
//    int expected_response_count_ = 3;
    int expected_response_count_ = 1;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
};


// New class derived from EvoCamoGame to test pre-trained FCD predator
class EvoCamoVsStaticFCD : public EvoCamoGame
{
public:
    EvoCamoVsStaticFCD(const CommandLine& cmd) : EvoCamoGame(cmd)
    {
        // Adjust "texture size" (prey diameter) to match GUI size.
        float canonical_gui_size = 1024;
        float canonical_texture_size = 201;
        float texture_to_gui_ratio = canonical_texture_size / canonical_gui_size;
        float gui_size = gui().getSize().x();
        std::cout << std::endl;
        debugPrint(gui_size);
        debugPrint(getTextureSize() / 4);
        debugPrint(int(texture_to_gui_ratio * gui_size));
        std::cout << std::endl;
        setTextureSize(int(texture_to_gui_ratio * gui_size));
    }

    // Ad hoc idle loop, sends request (as an image) to the "predator server"
    // then wait for its response. The predator's response is used the same way
    // as the user's mouse click is used in interactive version of simulation.
    void waitForUserInput() override
    {
        gui().refresh();
        // Wait for prediction from "predator server".
        int step = getStepCount();
        Vec2 prediction = getComms().performStep(step, gui().getCvMat());
        Vec2 prediction_in_pixels = prediction * gui().getSize().x();
        // Record predator's response and display on GUI.
        setLastMouseClick(prediction_in_pixels);
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20221012 enable draw crosshair
//        drawCrosshairAnnotation();
//        if (enableDrawCrosshair()) { drawCrosshairAnnotation(); }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        gui().refresh();
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20221014 formalize step save stride. [this comment needs updating]

        // Save an annotated tournament image every 19 steps (chosen to be
        // relatively prime to subpops, so we see results from all subpops)
        // or if requested with key command.
        if (getComms().keyTyped2())
            { std::cout << "    "; writePreviousStepImageToFile(); }
//        if (getComms().keyTyped1() || (step % 19 == 0))
        if (getComms().keyTyped1() || saveThisStep())
            { std::cout << "    "; writeTournamentImageToFile(); }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20221015 draw crosshair on "ps" window

        // Maintain a second window showing previous step outcome.
        std::string ps = "previous step";
        // Save previous step image.
        previous_step_image_ = gui().getCvMat().clone();
        // Save crosshairs on top of previous step image.
        drawCrosshairAnnotation();
        
//        cv::imshow("previous step", previous_step_image_);
//        cv::imshow(ps, previous_step_image_);
        cv::imshow(ps, gui().getCvMat());

        cv::setWindowTitle(ps, ps + " (" + getStepAsString() + ")");

        // Count and record "invalid tournaments" -- aka "predator fails"
        recordPredatorFailTimeSeriesData();
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20221012 run log
        appendStepResultsToRunLog();
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // Short wait (0.1 second) allowing the OpenCV windows to refresh.
        Texture::waitKey(100);
    }
    
    // Just a constant, but wrap in API so derived classes can access.
    int stepSaveStride() const { return step_save_stride_; }

    // Should this step's tournament image be saved? (Criteria for file saving.)
    bool saveThisStep() override
    {
        int step = getStepCount();
        // Normally save every n-th image (where N = 19).
        bool savable = (step % stepSaveStride()) == 0;
        // Save only at start and near end of a "superheavy" run (12000 steps).
        bool skip = (step > 100) and (step < 10000) and (getMaxSteps() > 10000);
        // Log a message when skipping a savable step.
        if (savable && skip)
        {
            std::cout << "skip mid-run saves for “super heavy” runs" << std::endl;
        }
        return savable && !skip;
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20221012 run log
    virtual void appendStepResultsToRunLog()
    {
        //int step = getPopulation()->getStepCount();
        //std::vector<Vec2> predator_responses = getComms().allResponses();
        //std::vector<Disk> prey_disks = getPreyDisks();
    }
    
    // TODO 20221013 move to EvoCamoVsStaticFCD
    // TODO 20221013 actually write log to file.
    
    // Add one line to end of run log file.
    void appendLineToRunLog(const std::string& line_text)
    {
        // Pathname for log file in this run's output directory.
        fs::path output_directory = outputDirectoryThisRun();
        fs::path output_pathname = output_directory / "run_log.txt";
        // Open output_stream to output_pathname in append mode.
        std::ofstream output_stream;
        output_stream.open(output_pathname, std::ios::app);
        // Append the given line of text to the log file.
        output_stream << line_text << std::endl;
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    

    // Draws a circle-and-crosshair for each response position returned by the
    // predator side. The first one is colored B&W as before, then six other
    // colors (WGRBYCM). These seven colors are repeated as needed.
    void drawCrosshairAnnotation()
    {
        Color white(1);
        Color black(0);
        std::vector<Color> colors
        {
            white,
            Color(0, 1, 0),
            Color(1, 0, 0),
            Color(0, 0, 1),
            Color(1, 1, 0),
            Color(0, 1, 1),
            Color(1, 0, 1)
        };
        float pixel_size = gui().getSize().x();
        std::vector<Vec2> responses = getComms().allResponses();
        for (int i = int(responses.size() - 1); i >= 0; i--)
        {
            Vec2 predator_response = responses.at(i);
            Color color = colors.at(i % colors.size());
            gui().drawDashedCircle(predator_response * pixel_size,
                                   textureSize() * 1.1,
                                   24,
                                   black,
                                   interpolate(0.5, color, white),
                                   true);
        }
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20221012 enable draw crosshair
    bool enableDrawCrosshair() const { return false; }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Count and record "invalid tournaments" -- aka "predator fails"
    void recordPredatorFailTimeSeriesData()
    {
        int step = getStepCount();
        if ((step % 10) == 0)
        {
            // Open output stream to file in append mode.
            fs::path out = outputDirectoryThisRun();
            std::ofstream outfile;
            outfile.open(out / "predator_fails.csv", std::ios::app);
            // Column headings for csv file.
            if (step == 0)
            {
                std::cout << "    " << "steps,fails" << std::endl;
                outfile << "steps,fails" << std::endl;
            }
            int fails = getPredatorFails();
//            std::cout << "    " << step << "," << fails << std::endl;
            std::cout << "    Predator fail rate: ";
            std::cout << std::setprecision(4) << fails / float(step);
            std::cout << " (" << fails << "/" << step  << ")" << std::endl;
            outfile << step << "," << fails << std::endl;
        }
    }

    // Like writeTournamentImageToFile() but save "previous step" image to file.
    void writePreviousStepImageToFile()
    {
        fs::path path = outputDirectoryThisRun();
        path /= "step_" + getStepAsString(-1) + ".png";
        writeTournamentImageToFile(path, previous_step_image_);
    }

    PythonComms& getComms() { return comms_; }
private:
    PythonComms comms_;
    cv::Mat previous_step_image_;
    int step_save_stride_ = 19;
};

// This is the version to support a "learning predator" which requires sending a
// file with 3 xy pairs (for the ground truth location of prey centers) used in
// fine-tuning/specializing the predator. The prey centers as given as floating
// point xy in relative image coordinates. (So always in [0, 1].)
//
class EvoCamoVsLearningPredator : public EvoCamoVsStaticFCD
{
public:
    EvoCamoVsLearningPredator(const CommandLine& cmd) : EvoCamoVsStaticFCD(cmd)
    {
        // Enable texture render timeout of 5 seconds.
        Texture::setRenderMaxTime(5);
    }

    // Write prey_n.txt file with three disk center positions ("xy3"), wait for
    // response, delete previous prey_n.txt file.
    void waitForUserInput() override
    {
        float image_size = gui().getCvMat().rows;
        int step = getStepCount();
        auto make_pathname = [&](int step)
        {
            return getComms().makePathname(step, "prey_", ".txt");
        };
        getComms().verifyCommsDirectoryReachable();
        std::ofstream output_file(make_pathname(step));
        for (auto disk : getPreyDisks())
        {
            output_file << disk.position.x() / image_size << " ";
            output_file << disk.position.y() / image_size << " ";
        }
        output_file << std::endl;
        output_file.close();

        // Wait for response.
        EvoCamoVsStaticFCD::waitForUserInput();
        
        // Delete prey_n.txt file from previous step.
        if (step > 0) { fs::remove(make_pathname(step - 1)); }
    }
    
    // Generate and store random non-overlapping prey disks in gui window.
    // Use EvoCamoGame::generatePreyPlacement() to generate a placement,
    // reject those that violate "avoid placement near center" constraint.
    void generatePreyPlacement() override
    {
        // Retry up to 200 times to fine one that meets the center constraint.
        int max_retries = 200;
        Vec2 center = guiSize() / 2;
        float min_dist = textureSize();
        for (int retry = 0; retry < max_retries; retry++)
        {
            // Use method from base class to make a candidate placement.
            EvoCamoGame::generatePreyPlacement();
            // Does this placement satisfy the "avoid center" constraint?
            bool all_avoid_center = true;
            for (auto& prey_disk : getPreyDisks())
            {
                float distance = (prey_disk.position - center).length();
                if (distance < min_dist) { all_avoid_center = false; }
            }
            // Exit retry loop if all disks avoid center zone.
            if (all_avoid_center) { break; }
        }
    }
};


// New EvoCamoVsLearnPredPop corresponding to EvoCamoVsLearnPredPop.ipynb in
// system PredatorEye.
class EvoCamoVsLearnPredPop : public EvoCamoVsLearningPredator
{
public:
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20221012 build out expected_response_count_ api
    
//    EvoCamoVsLearnPredPop(const CommandLine& cmd)
//    : EvoCamoVsLearningPredator(cmd) {}

    EvoCamoVsLearnPredPop(const CommandLine& cmd)
        : EvoCamoVsLearningPredator(cmd)
    {
        // Three predators per tournament, each producing an xy prediction.
        getComms().setExpectedResponseCount(3);
        
//        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        // TODO 20221013 actually write log to file.
//        appendLineToRunLog(getRunID());
//        appendLineToRunLog(cmd.wholeCommandAsString());
//        appendLineToRunLog("step, in_disk(i*3), "
//                           "predator prediction(x,y*3), "
//                           "prey position(x,y*3)");
//        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        
        
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // TODO 20221013 actually write log to file.
        whole_command_as_string_ = cmd.wholeCommandAsString();
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    // TODO 20221013 actually write log to file.
    // TODO 20221013 maybe move this to base class EvoCamoVsStaticFCD ?

    std::string whole_command_as_string_;
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // TODO 20221012 run log
    
//    void appendStepResultsToRunLog() override
//    {
//        int step = getPopulation()->getStepCount();
//        std::vector<Vec2> predator_responses = getComms().allResponses();
//        std::vector<Disk> prey_disks = getPreyDisks();
//
//
//        std::cout << "    LOG: ";
//        std::cout << step << ",";
//        std::cout << vec_to_string(predator_responses) << ",";
//
//        for (auto& disk : prey_disks)
//        {
//            std::cout << disk.position << ",";
//        }
//        std::cout << std::endl;
//    }

    
    // TODO 20221013 fix commas, add in_disk

//    void appendStepResultsToRunLog() override
//    {
//        int step = getPopulation()->getStepCount();
//        std::vector<Vec2> predator_responses = getComms().allResponses();
//        std::vector<Disk> prey_disks = getPreyDisks();
//
//
//        std::cout << "    LOG: ";
//        std::cout << step << ",";
//        std::cout << vec_to_string(predator_responses) << ",";
//
//        for (auto& disk : prey_disks)
//        {
//            std::cout << disk.position << ",";
//        }
//        std::cout << std::endl;
//    }
    
    // DELETEME copy for reference:
    
//    // Format float to a string, as percentage with "digits" fractional digits after
//    // the decimal point.
//    inline std::string float_to_percent_fractional_digits(float value, int digits)
//    {
//        std::stringstream ss;
//        ss << std::setprecision(2 + digits);
//        ss << 100 * value << "%";
//        return ss.str();
//    }

  
//        void appendStepResultsToRunLog() override
//        {
//            int step = getPopulation()->getStepCount();
//            std::vector<Vec2> predator_responses = getComms().allResponses();
//            std::vector<Disk> prey_disks = getPreyDisks();
//
//
//            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//            // TODO 20221013 actually write log to file.
//            if (step == 0)
//            {
//                appendLineToRunLog(getRunID());
//                appendLineToRunLog(whole_command_as_string_);
//                appendLineToRunLog("stride 19");
//                appendLineToRunLog("diameter " + std::to_string(diskDiameter()));
//    //            appendLineToRunLog("step, in_disk(i*3), "
//    //                               "predator prediction(x,y*3), "
//    //                               "prey position(x,y*3)");
//                appendLineToRunLog("each line below: step number, "
//                                   "3 in_disk bits, "
//                                   "3 xy predator preditions, "
//                                   "3 xy prey center positions");
//                appendLineToRunLog("step, in1,in2,in3, "
//                                   "p1x,p1y,p2x,p2y,p3x,p3y, "
//                                   "c1x,c1y,c2x,c2y,c3x,c3y");
//                appendLineToRunLog("data");
//            }
//
//            // "step,in1,in2,in3,px1,py1,px2,py2,px3,py3,cx1,cy1,cx2,cy2,cx3,cy3"
//            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//
//
//            std::stringstream ss;
//            std::string s = ",";
//
//    //        ss << step << ",";
//    //        ss << vec_to_string(predator_responses) << ",";
//    //
//    //        for (auto& disk : prey_disks)
//    //        {
//    //            ss << disk.position << ",";
//    //        }
//
//            auto log_xy = [&](Vec2 xy) { ss << s << xy.x() << s << xy.y(); };
//
//            ss << step;
//            ss << s << "0" << s << "0" << s << "0";
//            for (auto& r : predator_responses) { log_xy(r); }
//            for (auto& disk : prey_disks) { log_xy(disk.position); }
//            appendLineToRunLog(ss.str());
//        }
    
//        // TODO 20221014 Prototype to be refined and moved to base class.
//        // Convert a position expressed as image relative coordinates (that
//        // is: ([0,1],[0,1])) to the equivalent position in pixel units.
//    //    Vec2 imageRelativePositionToPixels(Vec2 v) const
//    //    Vec2 imageNormalizedToPixels(Vec2 v) const
//        Vec2 toPixels(Vec2 position_in_image_normalized_coordiantes ) const
//        {
//            return v * gui().getSize().x();
//        }
    


    
//        // TODO 20221014, from EvoCamoVsStaticFCD::waitForUserInput()
//        // TODO 20221014, back-patch that.
//        //     Vec2 prediction_in_pixels = prediction * gui().getSize().x();
//
//        // TODO 20221014 Prototype to be refined and moved to base class.
//        // Convert a position expressed as image relative coordinates (that
//        // is: ([0,1],[0,1])) to the equivalent position in pixel units.
//        //    Vec2 imageRelativePositionToPixels(Vec2 v) const
//        //    Vec2 imageNormalizedToPixels(Vec2 v) const
//        Vec2 toPixels(Vec2 position_in_image_normalized_coordinates) const
//        {
//            auto image_width = gui().getSize().x();
//            return position_in_image_normalized_coordinates * image_width;
//        }
//
//        void appendStepResultsToRunLog() override
//        {
//            if (saveThisStep())
//            {
//                int step = getPopulation()->getStepCount();
//                if (step == 0)
//                {
//                    appendLineToRunLog(getRunID());
//                    appendLineToRunLog(whole_command_as_string_);
//
//    //                appendLineToRunLog("stride 19");
//                    appendLineToRunLog("stride " + std::to_string(stepSaveStride()));
//
//
//
//                    appendLineToRunLog("diameter " + std::to_string(diskDiameter()));
//                    appendLineToRunLog("image_size " +
//                                       std::to_string(int(gui().getSize().x())) +
//                                       " " +
//                                       std::to_string(int(gui().getSize().y())));
//                    appendLineToRunLog("# each line below: step number, "
//                                       "3 in_disk bits, "
//                                       "3 xy predator predictions, "
//                                       "3 xy prey center positions");
//                    appendLineToRunLog("# step, in1,in2,in3, "
//                                       "p1x,p1y,p2x,p2y,p3x,p3y, "
//                                       "c1x,c1y,c2x,c2y,c3x,c3y");
//                    appendLineToRunLog("data");
//                }
//    //            std::string s = ",";
//                std::string s = " ";
//                std::stringstream ss;
//                std::vector<Disk> prey_disks = getPreyDisks();
//                std::vector<Vec2> predator_responses = getComms().allResponses();
//                auto log_xy = [&](Vec2 xy) { ss << s << xy.x() << s << xy.y(); };
//                ss << step;
//                ss << s << "0" << s << "0" << s << "0";
//                for (auto& r : predator_responses) { log_xy(toPixels(r)); }
//                for (auto& disk : prey_disks) { log_xy(disk.position); }
//                appendLineToRunLog(ss.str());
//            }
//        }

    
    // TODO 20221014, from EvoCamoVsStaticFCD::waitForUserInput()
    // TODO 20221014, back-patch that.
    //     Vec2 prediction_in_pixels = prediction * gui().getSize().x();
    
    
    void appendStepResultsToRunLog() override
    {
        if (saveThisStep())
        {
            int step = getStepCount();
            if (step == 0)
            {
                appendLineToRunLog(getRunID());
                appendLineToRunLog(whole_command_as_string_);
                appendLineToRunLog("stride " + std::to_string(stepSaveStride()));
                appendLineToRunLog("diameter " + std::to_string(diskDiameter()));
                appendLineToRunLog("image_size " +
                                   std::to_string(int(gui().getSize().x())) +
                                   " " +
                                   std::to_string(int(gui().getSize().y())));
                appendLineToRunLog("# each line below: step number, "
                                   "3 in_disk bits, "
                                   "3 xy predator predictions, "
                                   "3 xy prey center positions");
                appendLineToRunLog("# step  in1 in2 in3  "
                                   "p1x p1y p2x p2y p3x p3y  "
                                   "c1x c1y c2x c2y c3x c3y");
                appendLineToRunLog("data");
            }
            std::string s = " ";
            std::stringstream ss;
            std::vector<Disk> prey_disks = getPreyDisks();
//            std::vector<Vec2> predator_responses = getComms().allResponses();
            std::vector<Vec2> predator_responses =
                toPixels(getComms().allResponses());
            
            std::vector<int> in_disks =
                predictionInAnyDisk(predator_responses, prey_disks);

            auto log_xy = [&](Vec2 xy) { ss << s << xy.x() << s << xy.y(); };
            ss << step << s;
            
//            ss << s << "0" << s << "0" << s << "0" << s;
            for (auto& in_disk : in_disks) { ss << s << in_disk; }
            ss << s;

            
//            for (auto& r : predator_responses) { log_xy(toPixels(r)); }
            for (auto& r : predator_responses) { log_xy(r); }
            ss << s;
            for (auto& disk : prey_disks) { log_xy(disk.position); }
            appendLineToRunLog(ss.str());
        }
    }
    
    // Check to see if each predator prediction is inside any prey disk.
    // Returns a collection of 1 for yes and 0 for no, in order of predictions.
    std::vector<int> predictionInAnyDisk(const std::vector<Vec2>& predictions,
                                         const std::vector<Disk>& prey_disks)
    {
        std::vector<int> result;
        
        for (auto& prediction : predictions)
        {
            int in = 0;
            for (auto& prey_disk : prey_disks)
            {
                if (prey_disk.isInside(prediction)) in = 1;
            }
            result.push_back(in);
        }
        return result;
    }


    
    // TODO 20221014 Prototype to be refined and moved to base class.
    // Convert a position expressed as image relative coordinates (that
    // is: ([0,1],[0,1])) to the equivalent position in pixel units.
    //    Vec2 imageRelativePositionToPixels(Vec2 v) const
    //    Vec2 imageNormalizedToPixels(Vec2 v) const
    Vec2 toPixels(Vec2 position_in_image_normalized_coordinates) const
    {
        auto image_width = gui().getSize().x();
        return position_in_image_normalized_coordinates * image_width;
    }
    // Convert positions in normalized image coordinates to pixels
    std::vector<Vec2> toPixels(const std::vector<Vec2>& positions) const
    {
        std::vector<Vec2> result;
        for (auto& p : positions) { result.push_back(toPixels(p)); }
        return result;
    }
    
    
//    // TODO 20221013 move to EvoCamoVsStaticFCD
//    // TODO 20221013 actually write log to file.
//    // Add one line to end of run log file.
//    void appendLineToRunLog(const std::string& line_text)
//    {
//        // Pathname for log file in this run's output directory.
//        fs::path output_directory = outputDirectoryThisRun();
//        fs::path output_pathname = output_directory / "run_log.txt";
//        // Open output_stream to output_pathname in append mode.
//        std::ofstream output_stream;
//        output_stream.open(output_pathname, std::ios::app);
//        // Append the given line of text to the log file.
//        output_stream << line_text << std::endl;
//    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    // TODO 20221014 mocking API which should be way down in the base class.
//    // Equivalent to getTextureSize(). Unify?
//    int diskDiameter() {return 101;}
//
//    debugPrint(getTextureSize())
//    debugPrint(diskDiameter())
//    getTextureSize() = 100
//    diskDiameter() = 101

    
    // Diameter of prey disks, in pixels.
    // TODO 20221015 should be way down in some base class. Maybe EvoCamoGame?
    // TODO 20221015 maybe replace getTextureSize() with diskDiameter()?
    int diskDiameter() const { return getTextureSize(); }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

};

// TODO 20230308 prototype “Static Quality Metric” via pre-trained predator.

// March 8, 2023
//
// OK I know this class name is ridiculous, but EvoCamoVsLppSqm translates to:
//     Evolutionary Camouflage
//     Versus Learning Predator Population
//     with a Static Quality Metric
//
// This is just a variant thrown together a few days before the ALIFE 2023
// submission deadline (on 20230313). I hope to use an average of evaluations
// made by a “standard predator” (the “pre-trained but not fine-tuned” generic
// FCD predator) to provide a static quality metric of camouflage unrelated to
// the evolving/learning population of predators.
//
class EvoCamoVsLppSqm : public EvoCamoVsLearnPredPop
{
public:
    
    EvoCamoVsLppSqm(const CommandLine& cmd) : EvoCamoVsLearnPredPop(cmd) {}
    
    // Using this method name just as a "hook", adding a "before" method to it.
    void waitForUserInput() override
    {
        // Handle the Static Quality Metric protocol.
        handleSQM();

        // Call corresponding method in base class.
        EvoCamoVsLearnPredPop::waitForUserInput();
    }

    // Occasionally log SQM data.
    // TODO 20231004 Left over from 20230308 prototype.
    //               Refactor? Rename to logForSQM()? Maybe fold into logSQM()?
    const int sqm_interval_ = 100;
    void handleSQM()
    {
        // TODO 20230928 auto-curate part 2
        //               note that this temp logging should be removed
        std::cout << "    ++++ SQMs for tournament_group: ";
        for (const auto& member : getTournamentGroup().members())
        {
            std::cout << getPreySQM(member.individual) << ", ";
        }
        std::cout << std::endl;
        if ((getStepCount() % sqm_interval_) == 0) { logSQM(); }
    }

    // Get the SQM from a prey Individual, computing and caching it is needed.
    float getPreySQM(Individual* individual)
    {
        if (! individual->hasStaticQualityMetric())
        {
            // Save GUI.
            cv::Mat gui_save = gui().getCvMat().clone();
            evaluateIndividualSQM(*individual);
            // Restore GUI.
            gui().drawMat(gui_save, Vec2());
        }
        return individual->getStaticQualityMetric();
    }

    // Should this step's tournament image be saved? Defines criteria for image
    // saving during simulation. For auto-curation it is when all three prey
    // have perfect SQM of 1.0 and all three predators fail to find prey.
    bool saveThisStep() override
    {
        // Assume we will save.
        bool save = true;
        // Do not save if any SQM is less than 99%
        for (const auto& member : getTournamentGroup().members())
        {
            if (getPreySQM(member.individual) < 0.99) { save = false; }
        }
        // Do not save if any predators have sucessfully located prey.
        if (getTournamentGroup().getValid()) { save = false; }
        return save;
    }

    // For auto-curate: need to revisit saveThisStep() AFTER the tournament has
    // been completed, and possibly be declared invalid if all three predators
    // failed to find prey. THAT is what sets getTournamentGroup().getValid() to
    // be false.
    void invalidTournamentGroupHook() override
    {
        if (saveThisStep())
        {
            std::cout << "    ++++ (saving tournament image) ";
            writePreviousStepImageToFile();
        }
    }

    // An SQM measurement is based on this many trials. (Had been 10 for plots
    // in the ALIFE paper. Reduced it for use in auto-curate. 5 is probably good
    // enough but more evaluation might be helpful.)
    int trialsPerSQM() const { return 5; }

    // Set prey individual's SQM as the average of several trials.
    void evaluateIndividualSQM(Individual& individual)
    {
        float sum_of_trials = 0;
        for (int i = 0; i < trialsPerSQM(); i++)
        {
            sum_of_trials += sampleSQM(individual);
        }
        float average_metric = sum_of_trials / trialsPerSQM();
        // Cache average SQM on this prey, a LazyPredator Population Individual.
        individual.setStaticQualityMetric(average_metric);
    }
    
    // Constructs a test image and returns its Static Quality Metric.
    float sampleSQM(Individual& individual)
    {
        // Make test image of this prey over a randomly selected background.
        // send it to PredatorEye for evaluation
        Vec2 prey_center = makeImageForEvaluation(individual);
        
        sendForEvaluation();

        Vec2 predator_prediction = waitForEvaluation();
        float aim_error = (predator_prediction - prey_center).length();
        float disk_radius = diskDiameter() / (2 * guiSize().x());

        // Is prediction outside prey disk?
        return (aim_error > disk_radius) ? 1 : 0;
    }

    // Constructs SQM composite image, leaving it in GUI. Return center point.
    Vec2 makeImageForEvaluation(Individual& individual)
    {
        cv::Mat background_image = selectRandomBackgroundForWindow();
        
        // Draw the randomly selected background, then the 3 textures on top.
        gui().drawMat(background_image, Vec2());

        int size = textureSize();
        Vec2 size2d(size, size);

        Vec2 tl = size2d / 2;
        Vec2 br = guiSize() - (size2d * 1.5);
        Vec2 prey_tl = rs_.randomPointInAxisAlignedRectangle(tl, br);
        Vec2 prey_center = prey_tl + tl;
    
        Texture* texture = GP::textureFromIndividual(&individual);
        texture->rasterizeToImageCache(size, true);
        cv::Mat target = gui().getCvMatRect(prey_tl, size2d);
        texture->matteImageCacheDiskOverBG(size, target);
        
        mife_counter_++;
        gui().refresh();  // QQQ
        return prey_center / guiSize().x();
    }
          
    void sendForEvaluation()
    {
        // Helper function to make fs:path from preprefix ("wait_" or "eval_")
        auto p = [&](std::string preprefix)
        {
            int step = getStepCount();
            std::string prefix = preprefix + std::to_string(mife_counter_) + "_";
            return getComms().makePathname(step, prefix, ".png");
        };
        // Write image file with temp name.
        getComms().writeFile(p("wait_"),
                             getComms().resizeImage(gui().getCvMat()));
        // Now rename from "wait_..." to "eval_..."
        fs::rename(p("wait_"), p("eval_"));
    }

    // Wait for response from SQM evaluation, parse results, delete temp files.
    Vec2 waitForEvaluation()
    {
        // Poll and sleep until file appears.
        int step = getStepCount();
        std::string prefix = "sqm_" + std::to_string(mife_counter_) + "_";
        fs::path path = getComms().makePathname(step, prefix, ".txt");
        while (!getComms().isFilePresent(path))
        {
            using namespace std::chrono_literals;  // QQQ
            std::this_thread::sleep_for(0.01s);  // QQQ
        }
        // Read response from PredatorEye, xy prediction from standard predator.
        std::vector<Vec2> xy_points = getComms().readResponseFile(path, 1);
        
        // 20231003 Fix bug where the last response file (eg "sqm_15_0.txt") for
        // each step was not being deleted from comms directory. The correct fix
        // would probably be in PredatorServer.process_any_sqm_files() in the
        // PredatorEye repository since that is where the other response files
        // are deleted (delete-if-exists). But I am reluctant to touch that for
        // the first time in 9 months, lest I break something. This will do.
        fs::remove(path);

        // Delete corresponding eval_...png file now that it has been evaluated.
        std::string pf2 = "eval_" + std::to_string(mife_counter_) + "_";
        fs::path eval_image_path = getComms().makePathname(step, pf2, ".png");
        fs::remove(eval_image_path);

        // Return prediction from standard predator.
        assert(!xy_points.empty());
        return xy_points.front();
    }
    
    // Called very 100 steps, to log SQM max and average over the population.
    void logSQM()
    {
        int step = getStepCount();
        // Open output stream to file in append mode.
        fs::path out = outputDirectoryThisRun();
        std::ofstream outfile;
        outfile.open(out / "sqm_log.csv", std::ios::app);
        // Column headings for csv file.
        if (step == 0) { outfile << "step,average,max" << std::endl; }
        float ave = averageStaticQualityMetric();
        float max = maxStaticQualityMetric();
        outfile << step << "," << ave << "," << max << std::endl;
        outfile.close();
        std::cout << step << ": SQM average " << ave
                  << ", SQM max " << max << std::endl;
    }

    // Average SQM over entire prey population.
    float averageStaticQualityMetric() const
    {
        float total = 0;
        auto f = [&](Individual* i){ total += i->getStaticQualityMetric(); };
        getPopulation()->applyToAllIndividuals(f);
        return total / getPopulation()->getIndividualCount();
    }
    
    // Maximum SQM over entire prey population.
    float maxStaticQualityMetric() const
    {
        float max = -std::numeric_limits<float>::infinity();
        auto f = [&](Individual* i)
        {
            float sqm = i->getStaticQualityMetric();
            if (max < sqm) { max = sqm; }
        };
        getPopulation()->applyToAllIndividuals(f);
        return max;
    }

private:
    RandomSequence rs_;
    int mife_counter_ = 0;
};

//~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~ ~~
