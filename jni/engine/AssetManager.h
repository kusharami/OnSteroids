/*
 * AssetManager.h
 *
 *  Класс для работы с файлами из Java-среды.
 *
 *  Дата создания: 02.09.2013
 *  Дата изменения: 16.09.2013
 *
 *  Автор: Черданцева Александра
 */

#ifndef ASSETMANAGER_H_
#define ASSETMANAGER_H_

#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <stdio.h>

class AssetFile
{
	public:
		AssetFile(AAssetManager *mgr, const char* fileName);
		virtual ~AssetFile();

		int Read(void* buf, int size);
		off_t Seek(off_t offset, int origin);
		off_t Position();
		off_t Size();
	protected:
		AAsset* asset;
};

class AssetManager
{
	public:
		AssetManager(AAssetManager *mgr);
		AssetManager(JNIEnv* env, jobject activity);

		AssetFile* Open(const char* fileName);
	protected:
		AAssetManager* mgr;
};


#endif /* ASSETMANAGER_H_ */
