//
//  Camouflage.h
//  TexSyn
//
//  Created by Craig Reynolds on 1/20/21.
//  Copyright © 2021 Craig Reynolds. All rights reserved.
//
//------------------------------------------------------------------------------
//
// Early protoyping of an "Interactive Evolution of Camouflage" implementation
// on top of TexSyn and LazyPredator. Slightly updated version of the approach
// used in:
//         Craig Reynolds. 2011. Interactive Evolution of Camouflage.
//         Artificial Life 17(2). https://doi.org/10.1162/artl_a_00023
//
// Prototyping inside TexSyn, maybe should be its own library/git repository.
//
//------------------------------------------------------------------------------

#pragma once
#include "GP.h"

class Camouflage
{
public:
    // TODO original constructor for testing. Needs to be updated for newer
    //      parameters handled by argc/argv constructor. Maybe that one should
    //      reformat command line inputs to a version of this constructor
    //      rewritten to have ALL the bells and whistles.
    Camouflage(std::string run_name,
               std::string background_image_directory,
               float background_scale)
      : run_name_(run_name),
        background_image_directory_(background_image_directory),
        background_scale_(background_scale),
        gui_(gui_size_, Vec2())
    {}
    // This constructor parses a "unix style" command line for parameters.
    Camouflage(int argc, const char* argv[])
      : cmd_(argc, argv),
        run_name_(runNameDefault()),
        background_image_directory_(cmd_.positionalArgument(1)),
        output_directory_(cmd_.positionalArgument(2, ".")),
        output_directory_this_run_(runOutputDirectory()),
        background_scale_(cmd_.positionalArgument(3, float(0.5))),
        random_seed_(cmd_.positionalArgument(4, int(LPRS().defaultSeed()))),
        gui_size_(cmd_.positionalArgument(5, 1200),
                  cmd_.positionalArgument(6, 800)),
        gui_(gui_size_, Vec2()),
        individuals_(cmd_.positionalArgument(7, 120)),
        subpops_(cmd_.positionalArgument(8, 6)),
        max_init_tree_size_(cmd_.positionalArgument(9, 100)),
        min_crossover_tree_size_(max_init_tree_size_ * 0.5),
        max_crossover_tree_size_(max_init_tree_size_ * 1.5)
    {
        if (background_image_directory_.empty())
        {
            // Exit with failure after listing command arguments.
            std::cout << cmd_.positionalArgument(0);
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
            std::cout << "    max_tree_size (defaults to 100)" << std::endl;
            std::cout << "    min_crossover_tree_size (default 50)"<<std::endl;
            std::cout << "    max_crossover_tree_size (default 150)"<<std::endl;
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
        std::cout << "Reading " << background_image_filenames.size();
        std::cout << " background images:" << std::endl;
        int min_x = std::numeric_limits<int>::max();
        int min_y = std::numeric_limits<int>::max();
        for (auto& filename : background_image_filenames)
        {
            // Compose absolute pathname for this background image file.
            std::string pathname = backgroundImageDirectory() + "/" + filename;
            std::cout << "    " << pathname << std::endl;
            // Read the image file into an OpenCV image.
            cv::Mat bg = cv::imread(pathname);
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
        Population population(individuals_,
                              subpops_,
                              max_init_tree_size_,
                              min_crossover_tree_size_,
                              max_crossover_tree_size_,
                              GP::fs());
        // Read specified background image files, save as cv::Mats.
        collectBackgroundImages();
        std::filesystem::path out = output_directory_this_run_;
        std::cout << "Create output directory for this run: ";
        std::cout << out << std::endl;
        std::filesystem::create_directory(out);
        // Init GUI window.
        gui().setWindowName(run_name_);
        gui().refresh();
        // Loop "forever" performing interactive evolution steps.
        while (true)
        {
            // Store to allow access in mouse handler.
            step_ = population.getStepCount();
            // Evolution step with wrapped Camouflage::tournamentFunction().
            population.evolutionStep([&]
                                     (TournamentGroup tg)
                                     { return tournamentFunction(tg); });
        }
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
            gui().drawRectangle(size2 * 2,
                                disks.at(p-1).position - size2,
                                Color(0.7));
            gui().drawCircle(min_center_to_center - size / 2,
                             disks.at(p-1).position,
                             Color(1));
            cv::Mat target = gui().getCvMatRect(position, size2);
            texture->matteImageCacheDiskOverBG(size, target);
        }
        gui().refresh();
        Texture::waitKey(2);
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
        float min_center_to_center = radius * ((2 * std::sqrt(2)) + 1);
        float margin = min_center_to_center - (radius * 2);
        auto overlap_viz = [&](const std::vector<Disk>& disks)
        {
            // testdraw(tg, disks, rect_min, rect_max, min_center_to_center);
        };
        // Initialize "global variables" used by mouse callback handler.
        tournament_group_ = tg;
        disks_ = Disk::randomNonOverlappingDisksInRectangle(3, radius, radius,
                                                            margin,
                                                            rect_min, rect_max,
                                                            LPRS(),
                                                            overlap_viz);
        // Draw a randomly selected background, then the 3 textures on top.
        cv::Mat bg = selectRandomBackgroundForWindow();
        gui().drawMat(bg, Vec2());
        int p = 0;
        for (auto& tgm : tg.members())
        {
            int size = textureSize();
            Texture* texture = GP::textureFromIndividual(tgm.individual);
            texture->rasterizeToImageCache(size, true);
            Vec2 center_to_ul = Vec2(1, 1) * size / 2;
            Vec2 position = disks_.at(p++).position - center_to_ul;
            cv::Mat target = gui().getCvMatRect(position, Vec2(size, size));
            texture->matteImageCacheDiskOverBG(size, target);
        }
        // Update the onscreen image. Wait for user to click on one texture.
        gui().refresh();
        setMouseCallbackForTournamentFunction();
        Individual* worst = selectIndividualFromMouseClick(waitForMouseClick());
        // Designate clicked Texture's Individual as worst of TournamentGroup.
        tg.designateWorstIndividual(worst);
        // Clear GUI, return updated TournamentGroup.
        gui().clear();
        gui().refresh();
        return tg;
    }
    
    // Ad hoc idle loop, waiting for mouse click. (Better if waited for event.)
    // Listens for and executes single character commands: "t" and "Q".
    Vec2 waitForMouseClick()
    {
        wait_for_mouse_click_ = true;
        int previous_key = cv::waitKeyEx(1);
        // Loop until mouse is clicked in window.
        while (wait_for_mouse_click_)
        {
            // Wait for 1/4 second, and read any key typed during that time.
            int key = cv::waitKey(250);  // 1/4 second (250/1000)
            // When newly-pressed (key down) event.
            if ((key > 0) && (key != previous_key))
            {
                // For "t" command: write whole window tournament image to file.
                if (key == 't') { writeTournamentImageToFile(); }
                // For "Q" command: exit app immediately.
                if (key == 'Q') { exit(EXIT_SUCCESS); }
            }
            previous_key = key;
        }
        return getLastMouseClick();
    }

    // Controls mouse behavior during a tournament.
    void setMouseCallbackForTournamentFunction()
    {
        auto mouse_callback =
        []
        (int event, int x, int y, int flags, void* userdata)
        {
            if (event == cv::EVENT_LBUTTONDOWN)
            {
                auto c = static_cast<Camouflage*>(userdata);
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
                }
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
    //
    // TODO perhaps writeTournamentImageToFile() and writeThumbnailImageToFile()
    // should share a common utility?
    void writeTournamentImageToFile()
    {
        // TODO maybe I need a way to say "get whole thing" (getCvMat()) ?
        cv::Mat image = gui().getCvMatRect(Vec2(), guiSize());
        std::filesystem::path path = output_directory_this_run_;
        // TODO Is there a more portable way to specify the extension?
        path /= "step_" + std::to_string(step_) + ".png";
        std::cout << "Writing tournament image to file " << path << std::endl;
        cv::imwrite(path, image);
    }
    
    // Write a "thumbnail" with a texture and its background neighborhood.
    //
    // TODO perhaps writeTournamentImageToFile() and writeThumbnailImageToFile()
    // should share a common utility?
    void writeThumbnailImageToFile(Individual* individual)
    {
        // Get index of this Individual within current TournamentGroup.
        int index = individualToTournamentIndex(individual);
        // Construct a name for the thumbnail image file.
        std::vector<std::string> suffix = {"_a", "_b", "_c"};
        std::string filename = ("thumbnail_" +
                                std::to_string(step_) +
                                suffix.at(index) +
                                // TODO portable way to specify the extension?
                                ".png");
        // Construct a reference into a rectangular window of GUI.
        Vec2 size2(textureSize(), textureSize());
        Vec2 center = disks_.at(index).position;
        cv::Mat image = gui().getCvMatRect(center - size2, size2 * 2);
        // Construct pathname for file.
        std::filesystem::path path = output_directory_this_run_;
        path /= filename;
        std::cout << "Writing thumbnail image to file " << path << std::endl;
        // Write file.
        cv::imwrite(path, image);
    }
    
    // Given an Individual, find its index within the current TournamentGroup.
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
    
    // TODO very temp
    int textureSize() const { return 201; }
    
    // Get/set position of most recent mouse (left) click in GUI.
    Vec2 getLastMouseClick() const { return last_mouse_click_; }
    void setLastMouseClick(Vec2 mouse_pos)
    {
        last_mouse_click_ = mouse_pos;
        wait_for_mouse_click_ = false;
    }
    
    // Get default run name from background_image_directory_.
    // (Provides consistent behavior with and without trailing "/".)
    std::string runNameDefault()
    {
        std::filesystem::path path = cmd_.positionalArgument(1);
        std::string fn = path.filename();
        return (fn != "") ? fn : std::string(path.parent_path().filename());
    }
    
    // A subdirectory under output_directory_ for results from this run.
    std::string runOutputDirectory()
    {
        std::filesystem::path run_output_dir = output_directory_;
        run_output_dir /= (run_name_ + "_" + date_hours_minutes());
        return run_output_dir;
    }

private:
    // Parsed version of the ("unix") command line that invoked this run.
    const CommandLine cmd_;
    // Name of run, perhaps same as directory holding background image files.
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
    
    //-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    // Note: the five variables below communicate "global" state with the mouse
    // callback handler. This is not thread safe and would need redesign for
    // multithreading. But then, since this connects directly to the GUI for an
    // interactive task, multithreading seems unlikely?
    //
    // Store position of most recent mouse (left) click in GUI.
    Vec2 last_mouse_click_;
    // True during wait for user to select one texture on screen.
    bool wait_for_mouse_click_;
    // Collection of Disks describing layout of Textures in GUI window.
    std::vector<Disk> disks_;
    // TournamentGroup with pointers to 3 textures of most recent tournament.
    TournamentGroup tournament_group_;
    // Set to step count of Population in run() loop.
    int step_ = 0;
    //-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
};
