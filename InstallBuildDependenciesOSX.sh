# install brew first if you don't already have it
# ruby -e "$(curl -fsSkL raw.github.com/mxcl/homebrew/go)"

brew install opencv --with-tbb --with-qt
brew install liblo

git submodule update --init --recursive
