#ifndef PACKAGES_CLASS_H
#define PACKAGES_CLASS_H
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "SteamWorkshop.h"
#include <thread>
#include <atomic>
#include "Tex.h"
#include "InputSystem.h"
class PackageSystem
{
public:
	PackageSystem();
	~PackageSystem();
	void GUI();
	void createItem(const std::string& title, const std::string& description,
		const std::string& previewPath, const std::string& contentPath);
    void GUIOpenVision();
private:
    SteamWorkshop workshop;
    std::atomic<float> progress; // Tracks upload progress
    bool isUploading;

    std::vector<SteamUGCDetails_t> subscribedItems; // Store retrieved items
    bool isRetrievingItems = false;  // Track retrieval state
    bool open = false;
    bool moveWindow = true;
    void retrieveSubscribedItems(); // Function to get items
};
void PackageSystem::GUIOpenVision()
{
    if (open) {
        if (moveWindow) {
            ImGui::SetNextWindowPos(ImVec2(300, 200), ImGuiCond_Always); // Force position update
            moveWindow = false;
        }
        GUI();
    }
    else {
        moveWindow = true;
    }
    if (InputSystem::getHold(Inputs::KeyLeftShift) && InputSystem::getHold(Inputs::KeyP)) {
        open = true;
    }
}

void PackageSystem::GUI() {
    ImGui::Begin("Packages", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    ImGui::Text("Ctrl-p to search open");
    static Texture bannerTexture("EngineAssets/SteamWorkshop_banner.png");
    static ImTextureID texture = reinterpret_cast<ImTextureID>(static_cast<intptr_t>(bannerTexture.ID));
    ImGui::Image(texture, ImVec2(1920 / 6, 620 / 6), ImVec2(0, 1), ImVec2(1, 0));
    // Show progress bar if uploading
    if (isUploading) {
        ImGui::Text("Uploading Item...");
        ImGui::ProgressBar(progress, ImVec2(200, 20));
    }

    // Button to refresh subscribed items
    if (ImGui::Button("Refresh Subscribed Items")) {
        retrieveSubscribedItems();
    }

    if (isRetrievingItems) {
        ImGui::Text("Retrieving subscribed items...");
    }
    else if (!subscribedItems.empty()) {
        ImGui::Text("Subscribed Workshop Items:");

        // Display each subscribed item in a list
        for (size_t i = 0; i < subscribedItems.size(); ++i) {
            const auto& item = subscribedItems[i];

            ImGui::Separator();
            ImGui::Text("Title: %s", item.m_rgchTitle);
            ImGui::Text("ID: %llu", item.m_nPublishedFileId);
            ImGui::Text("Size: %llu bytes", item.m_nFileSize);

            // Download button
            if (ImGui::Button(("Re-Download##" + std::to_string(i)).c_str())) {
                std::thread([this, item]() {
                    workshop.DownloadItemContent(item.m_nPublishedFileId);
                    }).detach();
            }
        }
    }
    else {
        ImGui::Text("No subscribed items found.");
    }

}


void PackageSystem::createItem(const std::string& title, const std::string& description,
    const std::string& previewPath, const std::string& contentPath) {
    isUploading = true;
    progress = 0.0f;

    std::thread uploadThread([this, title, description, previewPath, contentPath]() {
        workshop.CreateWorkshopItem(title, description, previewPath, contentPath);

        // Wait for a valid handle (Steam API may take time)
        while (workshop.GetUploadProgress() == 0.0f && !workshop.IsUpdateCompleted()) {
            SteamAPI_RunCallbacks();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // Monitor progress
        while (!workshop.IsUpdateCompleted()) {
            float newProgress = workshop.GetUploadProgress();
            if (newProgress > 0.0f) {
                progress = newProgress;
            }
            SteamAPI_RunCallbacks();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        progress = 1.0f;
        isUploading = false;
        });

    uploadThread.detach();
}

void PackageSystem::retrieveSubscribedItems() {
    isRetrievingItems = true;
    subscribedItems.clear();

    std::thread retrieveThread([this]() {
        if (!workshop.RetrieveSubscribedItems()) {
            std::cerr << "Failed to start Workshop query" << std::endl;
            isRetrievingItems = false;
            return;
        }

        // Wait for the query to complete
        while (!workshop.IsQueryCompleted()) {
            SteamAPI_RunCallbacks();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // Get the results
        subscribedItems = workshop.GetSubscribedItems();
        isRetrievingItems = false;
        });

    retrieveThread.detach();
}

PackageSystem::PackageSystem() : progress(0.0f), isUploading(false)
{
}

PackageSystem::~PackageSystem()
{
}
#endif
