// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "KriegerUserSettings.generated.h"

UCLASS()
class UKriegerUserSettings : public UGameUserSettings
{
	GENERATED_UCLASS_BODY()

	/** Applies all current user settings to the game and saves to permanent storage (e.g. file). */
	virtual void ApplySettings() OVERRIDE;

	int32 GetGraphicsQuality() const
	{
		return GraphicsQuality;
	}

	void SetGraphicsQuality(int32 InGraphicsQuality)
	{
		GraphicsQuality = InGraphicsQuality;
	}

	/** Gets current fullscreen mode */
	EWindowMode::Type GetCurrentFullscreenMode() const;

	/** Interface UGameUserSettings */
	virtual void SetToDefaults() OVERRIDE;

	/** Changes to CUSTOM low quality graphics */
	void SetLowQuality();

	/** Changes to CUSTOM mid quality graphics */
	void SetMidQuality();

	/** Changes to CUSTOM high quality graphics */
	void SetHighQuality();

private:

	/**
	 * Graphics Quality
	 *	0 = Low
	 *  1 = Mid
	 *	2 = High
	 */
	UPROPERTY(config)
	int32 GraphicsQuality;

};
