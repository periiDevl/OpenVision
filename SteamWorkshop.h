#include"steam_api.h"
#include"isteamugc.h"
#include<iostream>
#include"vector"
#ifdef _WIN32
#include <windows.h>
#else
#define MAX_PATH 260
#endif
class SteamWorkshop {
private:
    // Upload-related members
    CCallResult<SteamWorkshop, CreateItemResult_t> m_CreateItemResult;
    CCallResult<SteamWorkshop, SubmitItemUpdateResult_t> m_SubmitItemResult;
    PublishedFileId_t m_CreatedFileId = 0;
    UGCUpdateHandle_t m_UpdateHandle = 0;
    bool m_UpdateCompleted = false;
    std::string m_Title;
    std::string m_Description;
    std::string m_PreviewPath;
    std::string m_ContentPath;

    // Download-related members
    CCallResult<SteamWorkshop, SteamUGCQueryCompleted_t> m_QueryCompletedResult;
    std::vector<SteamUGCDetails_t> m_SubscribedItems;
    bool m_QueryCompleted = false;

public:
    SteamWorkshop() {}
    float GetUploadProgress() {
        uint64 processed = 0, total = 0;

        if (m_UpdateHandle == k_UGCUpdateHandleInvalid) {
            std::cerr << "Update handle is invalid!" << std::endl;
            return 0.0f; // Invalid handle, return 0
        }

        if (SteamUGC()->GetItemUpdateProgress(m_UpdateHandle, &processed, &total)) {
            if (total > 0) {
                return static_cast<float>(processed) / static_cast<float>(total);
            }
        }

        return 0.0f; // Return 0 if progress can't be determined
    }


    // Upload functionality
    bool CreateWorkshopItem(const std::string& title, const std::string& description,
        const std::string& previewPath, const std::string& contentPath) {
        if (!SteamUGC()) {
            std::cerr << "Steam UGC interface not available" << std::endl;
            return false;
        }

        m_Title = title;
        m_Description = description;
        m_PreviewPath = previewPath;
        m_ContentPath = contentPath;

        SteamAPICall_t hSteamAPICall = SteamUGC()->CreateItem(SteamUtils()->GetAppID(),
            k_EWorkshopFileTypeCommunity);
        m_CreateItemResult.Set(hSteamAPICall, this, &SteamWorkshop::OnCreateItemResult);
        return true;
    }

    bool IsUpdateCompleted() const {
        return m_UpdateCompleted;
    }

    // Download functionality
    bool RetrieveSubscribedItems() {
        if (!SteamUGC()) {
            std::cerr << "Steam UGC interface not available" << std::endl;
            return false;
        }

        UGCQueryHandle_t query = SteamUGC()->CreateQueryUserUGCRequest(
            SteamUser()->GetSteamID().GetAccountID(),
            k_EUserUGCList_Subscribed,
            k_EUGCMatchingUGCType_Items,
            k_EUserUGCListSortOrder_CreationOrderDesc,
            SteamUtils()->GetAppID(),
            SteamUtils()->GetAppID(),
            1
        );

        if (query == k_UGCQueryHandleInvalid) {
            std::cerr << "Failed to create UGC query" << std::endl;
            return false;
        }

        SteamAPICall_t hSteamAPICall = SteamUGC()->SendQueryUGCRequest(query);
        m_QueryCompletedResult.Set(hSteamAPICall, this, &SteamWorkshop::OnQueryCompleted);
        return true;
    }

    const std::vector<SteamUGCDetails_t>& GetSubscribedItems() const {
        return m_SubscribedItems;
    }

    bool IsQueryCompleted() const {
        return m_QueryCompleted;
    }

    bool DownloadItemContent(PublishedFileId_t itemId) {
        uint64 sizeOnDisk;
        char installFolder[MAX_PATH];
        uint32 folderSize = sizeof(installFolder);

        if (!SteamUGC()->GetItemInstallInfo(itemId, &sizeOnDisk, installFolder, folderSize, nullptr)) {
            std::cerr << "Failed to get install info for item " << itemId << std::endl;
            return false;
        }

        if (SteamUGC()->DownloadItem(itemId, true)) {
            std::cout << "Downloading item " << itemId << " to: " << installFolder << std::endl;
            return true;
        }

        std::cerr << "Failed to initiate download for item " << itemId << std::endl;
        return false;
    }

    std::string GetItemInstallPath(PublishedFileId_t itemId) {
        uint64 sizeOnDisk;
        char installFolder[MAX_PATH];
        uint32 folderSize = sizeof(installFolder);

        if (SteamUGC()->GetItemInstallInfo(itemId, &sizeOnDisk, installFolder, folderSize, nullptr)) {
            return std::string(installFolder);
        }
        return "";
    }

private:
    // Upload callbacks
    void OnCreateItemResult(CreateItemResult_t* pResult, bool bIOFailure) {
        if (bIOFailure || pResult->m_eResult != k_EResultOK) {
            std::cerr << "Failed to create workshop item: " << pResult->m_eResult << std::endl;
            return;
        }

        m_CreatedFileId = pResult->m_nPublishedFileId;
        std::cout << "Successfully created workshop item with ID: " << m_CreatedFileId << std::endl;
        BeginUpdateItem();
    }

    void OnSubmitItemUpdateResult(SubmitItemUpdateResult_t* pResult, bool bIOFailure) {
        if (bIOFailure || pResult->m_eResult != k_EResultOK) {
            std::cerr << "Failed to submit workshop item update: " << pResult->m_eResult << std::endl;
            return;
        }

        std::cout << "Successfully updated workshop item!" << std::endl;
        m_UpdateCompleted = true;
    }

    void BeginUpdateItem() {
        if (m_CreatedFileId == 0) {
            std::cerr << "No item ID available to update" << std::endl;
            return;
        }

        m_UpdateHandle = SteamUGC()->StartItemUpdate(SteamUtils()->GetAppID(), m_CreatedFileId);
        if (m_UpdateHandle == k_UGCUpdateHandleInvalid) {
            std::cerr << "Failed to start item update" << std::endl;
            return;
        }

        SteamUGC()->SetItemTitle(m_UpdateHandle, m_Title.c_str());
        SteamUGC()->SetItemDescription(m_UpdateHandle, m_Description.c_str());
        SteamUGC()->SetItemPreview(m_UpdateHandle, m_PreviewPath.c_str());
        SteamUGC()->SetItemContent(m_UpdateHandle, m_ContentPath.c_str());
        SteamUGC()->SetItemVisibility(m_UpdateHandle, k_ERemoteStoragePublishedFileVisibilityPublic);

        SteamAPICall_t hSteamAPICall = SteamUGC()->SubmitItemUpdate(m_UpdateHandle, "Initial upload");
        m_SubmitItemResult.Set(hSteamAPICall, this, &SteamWorkshop::OnSubmitItemUpdateResult);
    }

    // Download callback
    void OnQueryCompleted(SteamUGCQueryCompleted_t* pResult, bool bIOFailure) {
        if (bIOFailure || pResult->m_eResult != k_EResultOK) {
            std::cerr << "Query failed: " << pResult->m_eResult << std::endl;
            return;
        }

        m_SubscribedItems.clear();

        for (uint32 i = 0; i < pResult->m_unNumResultsReturned; i++) {
            SteamUGCDetails_t details;
            if (SteamUGC()->GetQueryUGCResult(pResult->m_handle, i, &details)) {
                m_SubscribedItems.push_back(details);
            }
        }

        SteamUGC()->ReleaseQueryUGCRequest(pResult->m_handle);
        m_QueryCompleted = true;
        std::cout << "Retrieved " << m_SubscribedItems.size() << " subscribed items" << std::endl;
    }
};