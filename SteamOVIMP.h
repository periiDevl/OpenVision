#pragma once
/*
#include "steam_api.h"
#include "isteamugc.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <thread>
PublishedFileId_t CreateAndUploadWorkshopItem(const char* modFolder, const char* modTitle, const char* modDescription)
{
	// Initialize Steamworks API
	if (!SteamAPI_Init())
	{
		// Handle initialization failure
		std::cerr << "SteamAPI_Init() failed" << std::endl;
		//return;
	}

	// Create a workshop item
	PublishedFileId_t publishedFileId = SteamUGC()->CreateItem(SteamUtils()->GetAppID(), k_EWorkshopFileTypeCommunity);
	if (publishedFileId == k_PublishedFileIdInvalid)
	{
		// Handle item creation failure
		std::cerr << "Failed to create workshop item" << std::endl;
		//return;
	}

	// Start updating the workshop item
	UGCUpdateHandle_t updateHandle = SteamUGC()->StartItemUpdate(SteamUtils()->GetAppID(), publishedFileId);
	if (updateHandle == k_UGCUpdateHandleInvalid)
	{
		// Handle update handle creation failure
		std::cerr << "Failed to start item update" << std::endl;
		//return;
	}

	// Create a workshop item

	// Start updating the workshop item

	// **Uncomment these lines to set the workshop item details**
	SteamUGC()->SetItemTitle(updateHandle, modTitle);
	//SteamUGC()->SetItemDescription(updateHandle, modDescription);

	// Set mod visibility
	//SteamUGC()->SetItemVisibility(updateHandle, k_ERemoteStoragePublishedFileVisibilityPublic);

	// Set mod content (folder containing mod files)
	//SteamUGC()->SetItemContent(updateHandle, modFolder);

	// Submit the item update (upload the mod to workshop)
	SteamAPICall_t submitCall = SteamUGC()->SubmitItemUpdate(updateHandle, "Uploaded my mod!");
	if (submitCall == k_uAPICallInvalid)
	{
		// Handle submission failure
		std::cerr << "Failed to submit item update" << std::endl;
		//return;
	}
	return publishedFileId;
	// Ensure callback gets called
	// Add your callback handling here if needed
}

void UpdateWorkshopItem(const char* modFolder, const char* modTitle, const char* modDescription, PublishedFileId_t publishedFileId) {
	// Start updating the workshop item
	UGCUpdateHandle_t updateHandle = SteamUGC()->StartItemUpdate(SteamUtils()->GetAppID(), publishedFileId);
	if (updateHandle == k_UGCUpdateHandleInvalid) {
		// Handle update handle creation failure
		std::cerr << "Failed to start item update" << std::endl;
		return;
	}

	// Set the mod title and description
	SteamUGC()->SetItemTitle(updateHandle, "Hello");
	//SteamUGC()->SetItemDescription(updateHandle, modDescription);

	// Set mod visibility
	//SteamUGC()->SetItemVisibility(updateHandle, k_ERemoteStoragePublishedFileVisibilityPublic);

	// Set mod content (folder containing mod files)
	//SteamUGC()->SetItemContent(updateHandle, modFolder);

	// Submit the item update (upload the mod to workshop)
	SteamAPICall_t submitCall = SteamUGC()->SubmitItemUpdate(updateHandle, "Uploaded my mod!");
	if (submitCall == k_uAPICallInvalid) {
		// Handle submission failure
		std::cerr << "Failed to submit item update" << std::endl;
		return;
	}

	// Wait until the update is completed
	while (true) {
		// Poll for callback
		SteamAPI_RunCallbacks();

		// Check if the item update is completed
		EItemUpdateStatus updateStatus = SteamUGC()->GetItemUpdateProgress(updateHandle, nullptr, nullptr);

		if (updateStatus == k_EItemUpdateStatusCommittingChanges) {
			std::cout << "Updating workshop item..." << std::endl;
			// You can add more code here if you want to do something while the update is in progress
		}
		else if (updateStatus == k_EItemUpdateStatusCommittingChanges) {
			std::cout << "Workshop item updated successfully!" << std::endl;
			break;
		}
		else if (updateStatus == k_EItemUpdateStatusInvalid) {
			std::cerr << "Failed to update workshop item" << std::endl;
			break;
		}

		// Sleep for a short duration to avoid busy waiting
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void Steam(bool b_RSteam, bool b_Workshop)
{
	if (b_RSteam){ SteamAPI_Init(); }

	if (b_Workshop) {
		const char* modFolder = "F:/Development/C++/OpenVision/OpenVision(Engine)/Mods/Bullet";
		const char* modTitle = "My Awesome Mod";
		const char* modDescription = "This mod enhances gameplay with new features.";

		PublishedFileId_t publishedFileId = CreateAndUploadWorkshopItem(modFolder, modTitle, modDescription);
		if (publishedFileId == k_PublishedFileIdInvalid) {
			SteamAPI_Shutdown();
		}

		SteamAPI_RunCallbacks();

		UpdateWorkshopItem(modFolder, modTitle, modDescription, publishedFileId);

		SteamAPI_Shutdown();



		//PublishedFileId_t publishedFileId = SteamUGC()->CreateItem(2472350, k_EWorkshopFileTypeCommunity);

		UGCUpdateHandle_t updateHandle = SteamUGC()->StartItemUpdate(2472350, publishedFileId);

		SteamUGC()->SetItemTitle(updateHandle, "My Awesome Cat Spray");


		SteamAPICall_t hSteamAPICall = SteamUGC()->CreateItem(SteamUtils()->GetAppID(), k_EWorkshopFileTypeCommunity);

	}

}*/