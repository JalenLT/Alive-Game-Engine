#pragma once
#ifndef FILEBROWSERWINDOW_H
#define FILEBROWSERWINDOW_H

#include "UIWindow.h"
#include <cstdlib>
#include <filesystem>
#include <string>
#include "Event.h"

namespace fs = std::filesystem;

class FileBrowserWindow : public UIWindow {
public:
	FileBrowserWindow(float x = 0.0f, float y = 0.0f, float width = 300.0f, float height = 400.0f);

	fs::directory_iterator getDirectoryIterator(const fs::path& directory);

	void createFolder(const fs::path& path);

	void addFolder(const std::string name, const char* tooltip, bool goBack = false);

	void addFile(const std::string name, const std::string ext);
	
	void render() override;

private:
	std::string basePath;
	std::string currentPath;
};

#endif