class WorkshopUploader {
private:
    // Callback handles
    CCallResult<WorkshopUploader, CreateItemResult_t> m_CreateItemResult;
    CCallResult<WorkshopUploader, SubmitItemUpdateResult_t> m_SubmitItemResult;
    PublishedFileId_t m_CreatedFileId = 0;
    UGCUpdateHandle_t m_UpdateHandle = 0;

    bool m_UpdateCompleted = false;  // Private member to track update status

public:
    WorkshopUploader() {}

    // Getter function to check if the update is completed
    bool IsUpdateCompleted() const {
        return m_UpdateCompleted;
    }

    void OnCreateItemResult(CreateItemResult_t* pResult, bool bIOFailure) {
        if (bIOFailure || pResult->m_eResult != k_EResultOK) {
            std::cerr << "Failed to create workshop item: " << pResult->m_eResult << std::endl;
            return;
        }

        m_CreatedFileId = pResult->m_nPublishedFileId;
        std::cout << "Successfully created workshop item with ID: " << m_CreatedFileId << std::endl;

        // Start the update process
        BeginUpdateItem();
    }

    void OnSubmitItemUpdateResult(SubmitItemUpdateResult_t* pResult, bool bIOFailure) {
        if (bIOFailure || pResult->m_eResult != k_EResultOK) {
            std::cerr << "Failed to submit workshop item update: " << pResult->m_eResult << std::endl;
            return;
        }

        std::cout << "Successfully updated workshop item!" << std::endl;

        // Mark the update as completed
        m_UpdateCompleted = true;
    }

    bool CreateWorkshopItem(const std::string& title, const std::string& description, const std::string& previewPath, const std::string& contentPath) {
        if (!SteamUGC()) {
            std::cerr << "Steam UGC interface not available" << std::endl;
            return false;
        }

        // Store the provided values
        m_Title = title;
        m_Description = description;
        m_PreviewPath = previewPath;
        m_ContentPath = contentPath;

        // Create the item
        SteamAPICall_t hSteamAPICall = SteamUGC()->CreateItem(SteamUtils()->GetAppID(), k_EWorkshopFileTypeCommunity);
        m_CreateItemResult.Set(hSteamAPICall, this, &WorkshopUploader::OnCreateItemResult);

        return true;
    }

private:
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

        // Set item properties
        SteamUGC()->SetItemTitle(m_UpdateHandle, m_Title.c_str());
        SteamUGC()->SetItemDescription(m_UpdateHandle, m_Description.c_str());
        SteamUGC()->SetItemPreview(m_UpdateHandle, m_PreviewPath.c_str());
        SteamUGC()->SetItemContent(m_UpdateHandle, m_ContentPath.c_str());

        // Set item visibility
        SteamUGC()->SetItemVisibility(m_UpdateHandle, k_ERemoteStoragePublishedFileVisibilityPublic);

        // Submit the update
        SteamAPICall_t hSteamAPICall = SteamUGC()->SubmitItemUpdate(m_UpdateHandle, "Initial upload");
        m_SubmitItemResult.Set(hSteamAPICall, this, &WorkshopUploader::OnSubmitItemUpdateResult);
    }

    std::string m_Title;
    std::string m_Description;
    std::string m_PreviewPath;
    std::string m_ContentPath;
};
