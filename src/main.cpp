#define cimg_use_jpeg
#define cimg_use_png
#include "CImg.h"
using namespace cimg_library;

#include <bits/stdc++.h>
using namespace std;

CImg<unsigned char> img;
int w, h, sharpness;

vector<vector<int>> cc;
vector<vector<int>> chunks;

int dx[4] = { 0, 0, 1, -1 };
int dy[4] = { 1, -1, 0, 0 };

// Pour ne pas sortir de la grille
bool is_valid(int x, int y) {
  return 0 <= x && x < h && 0 <= y && y < w;
}

// Pour tester si deux pixels ont des couleurs similaires
bool is_adj(int x1, int y1, int x2, int y2) {
  bool ok = true;
  for(int i = 0; i < 3; ++i) {
    int diff = abs(img(y1, x1, i) - img(y2, x2, i));
    ok = ok && diff <= sharpness;
  }
  return ok;
}

// Première passe
void dfs(int x, int y, int c) {
  if(cc[x][y] != -1)
    return;
  cc[x][y] = c;
  for(int i = 0; i < 4; ++i) {
    int nx = x + dx[i];
    int ny = y + dy[i];
    if(is_valid(nx, ny) && is_adj(x, y, nx, ny))
      dfs(nx, ny, c);
  }
}

// Deuxième passe
void flood_fill(int x, int y, int c) {
  if(chunks[x][y] != -1)
    return;
  chunks[x][y] = c;
  for(int i = 0; i < 4; ++i) {
    int nx = x + dx[i];
    int ny = y + dy[i];
    if(is_valid(nx, ny))
      flood_fill(nx, ny, c);
  }
}

int main(int argc, char** argv) {
  if(argc <= 2) {
    cout << "Arguments: <sharpness> <tolerance> [filename]" << endl;
    return 1;
  }
  // Load image
  string filename(argc == 3 ? "image.png" : argv[3]);
  cout << "Loading image " << filename << endl;
  CImg<unsigned char> src(filename.c_str());
  img = src;
  w = src.width();
  h = src.height();
  {
    stringstream ss(argv[1]);
    ss >> sharpness;
  }

  // Print information
  cout << "image width and height: " << w << "x" << h << endl;
  cout << "sharpness: " << sharpness << endl;

  // Connected components
  cc.assign(h, vector<int>(w, -1));
  vector< pair<int, int> > sizes;
  int c = 0;
  for(int i = 0; i < h; ++i)
    for(int j = 0; j < w; ++j) {
      if(cc[i][j] == -1) {
	sizes.push_back(make_pair(0, c));
	dfs(i, j, c++);
      }
      ++sizes[cc[i][j]].first;
    }
  cout << c << endl;

  // Fill with random colors
  srand(time(0));
  vector< array<int, 3> > colors(c);
  for(int i = 0; i < c; ++i)
    for(int j = 0; j < 3; ++j)
      colors[i][j] = rand()%256;

  for(int i = 0; i < h; ++i)
    for(int j = 0; j < w; ++j) {
      for(int k = 0; k < 3; ++k)
	img(j, i, k) = colors[cc[i][j]][k];
    }

  img.save_png("out_manycolors.png");

  // Fill with 2 colors
  if(c > 1) {
    sort(sizes.rbegin(), sizes.rend()); // Reverse sort
    colors.resize(2);
    
    for(int i = 2; i < c; ++i)
      for(int j = 0; j < 3; ++j)
	colors[sizes[i].second][j] = colors[sizes[1].second][j];

    for(int i = 0; i < h; ++i)
      for(int j = 0; j < w; ++j)
	for(int k = 0; k < 3; ++k)
	  img(j, i, k) = colors[cc[i][j]][k];

    img.save_png("out_2colors.png");

    // Detect coins
    chunks.assign(h, vector<int>(w, -1));
    for(int i = 0; i < h; ++i)
      for(int j = 0; j < w; ++j)
	if(cc[i][j] == sizes[0].second)
	  chunks[i][j] = 0;

    sizes.clear();
    sizes.push_back(make_pair(0, 0));
    c = 1;
    for(int i = 0; i < h; ++i)
      for(int j = 0; j < w; ++j) {
	if(chunks[i][j] == -1) {
	  sizes.push_back(make_pair(0, c));
	  flood_fill(i, j, c++);
	}
	++sizes[chunks[i][j]].first;
      }

    double tolerance;
    {
      stringstream ss(argv[2]);
      ss >> tolerance;
    }
    tolerance /= 100.0;

    vector<int> minX(c, 1e9), maxX(c, 0), minY(c, 1e9), maxY(c, 0);
    for(int i = 0; i < h; ++i) {
      for(int j = 0; j < w; ++j) {
	minX[chunks[i][j]] = min(minX[chunks[i][j]], i);
	minY[chunks[i][j]] = min(minY[chunks[i][j]], j);
	maxX[chunks[i][j]] = max(maxX[chunks[i][j]], i);
	maxY[chunks[i][j]] = max(maxY[chunks[i][j]], j);
      }
    }
    
    int coins = 0;
    for(int i = 1; i < c; ++i) {
      double expected_size = M_PI*abs(minX[i]-maxX[i])/4.0*abs(minY[i]-maxY[i])/4.0;
      if(sizes[i].first >= w*h*tolerance && (expected_size - sizes[i].first)/expected_size < 0.2)
	++coins;
    }

    cout << c << " chunks detected!" << endl;
    cout << coins << " coins detected!" << endl;

    
    colors.resize(c);
    for(int i = 1; i < c; ++i)
      for(int j = 0; j < 3; ++j)
	colors[i][j] = rand()%256;

    for(int i = 0; i < h; ++i)
      for(int j = 0; j < w; ++j) {
	for(int k = 0; k < 3; ++k)
	  img(j, i, k) = colors[chunks[i][j]][k];
      }
    img.save_png("out_chunks.png");
  }
  return 0;
}
