#pragma once

namespace tc {
	enum FileError {
		FE_accessDenied = 1,
		FE_fileEmpty,
		FE_fileNotFound,
		FE_readError,
		FE_unknownError,
	};
	enum LifetimeFilesID {
		LFI_toastCatSprite = 0
	};
}
