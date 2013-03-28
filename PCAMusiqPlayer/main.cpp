#include <string>

#include <QtGui/QApplication>
#include "MainWindow.h"

#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("live-video,l", "use live video (prosilica grabber)")
        //("include-path,i", po::value< vector<string> >(), "include path")
        //("input-file", po::value< vector<string> >(), "input file")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(vm.count("live-video"))
    {
        std::cout << "Live video mode. Not implemented ;)" << std::endl;
        exit(0);
    }*/

    if(a.arguments().length() < 4 || a.arguments().contains("-h") || a.arguments().contains("--help"))
    {
        std::cout << "Usage: " << argv[0] << " <video_source> <pca> <osc_send_uri>" << std::endl;
        std::cout << std::endl;
        std::cout << "video_source: \"camera-prosilica\", \"camera-opencv\", or path to video file" << std::endl;
        std::cout << "pca: path to pre-computed pca (generated with e.g. pcamusiq)" << std::endl;
        std::cout << "osc_send_uri: OSC address of MusicBox" << std::endl;
        std::cout << std::endl;
        std::cout << "Example: " << argv[0] << " camera ~/my_pca/ 127.0.0.1:50000" << std::endl;

        exit(0);
    }


    MainWindow w;
    w.show();

    return a.exec();
}
