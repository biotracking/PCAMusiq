#include <QtGui/QApplication>
#include "MainWindow.h"

#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char *argv[])
{
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

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
