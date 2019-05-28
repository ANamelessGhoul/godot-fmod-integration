/*************************************************************************/
/*  godot_fmod.h                                                         */
/*************************************************************************/
/*                                                                       */
/*       FMOD Studio module and bindings for the Godot game engine       */
/*                                                                       */
/*************************************************************************/
/* Copyright (c) 2019 Alex Fonseka                                       */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef GODOT_FMOD_H
#define GODOT_FMOD_H

#include "core/dictionary.h"
#include "core/map.h"
#include "core/node_path.h"
#include "core/object.h"
#include "core/reference.h"
#include "core/vector.h"
#include "scene/2d/canvas_item.h"
#include "scene/3d/spatial.h"
#include "scene/main/node.h"

#include "api/core/inc/fmod.hpp"
#include "api/core/inc/fmod_errors.h"
#include "api/studio/inc/fmod_studio.hpp"

class Fmod : public Object {
	GDCLASS(Fmod, Object);

	FMOD::Studio::System *system;
	FMOD::System *coreSystem;

	float distanceScale;

	Object *listener;

	bool nullListenerWarning = true;

	Map<String, FMOD::Studio::Bank *> banks;
	Map<String, FMOD::Studio::EventDescription *> eventDescriptions;
	Map<String, FMOD::Studio::Bus *> buses;
	Map<String, FMOD::Studio::VCA *> VCAs;
	Map<String, FMOD::Sound *> sounds;
	Map<FMOD::Sound *, FMOD::Channel *> channels;

	// keep track of one shot instances internally
	Vector<FMOD::Studio::EventInstance *> oneShotInstances;
	struct AttachedOneShot {
		FMOD::Studio::EventInstance *instance;
		Object *gameObj;
	};
	Vector<AttachedOneShot> attachedOneShots;

	// events not directly managed by the integration
	// referenced through uuids generated in script
	Map<String, FMOD::Studio::EventInstance *> unmanagedEvents;

	FMOD_3D_ATTRIBUTES get3DAttributes(FMOD_VECTOR pos, FMOD_VECTOR up, FMOD_VECTOR forward, FMOD_VECTOR vel);
	FMOD_VECTOR toFmodVector(Vector3 vec);
	void setListenerAttributes();
	void updateInstance3DAttributes(FMOD::Studio::EventInstance *i, Object *o);
	int checkErrors(FMOD_RESULT result);
	bool isNull(Object *o);
	void loadBus(const String &busPath);
	void loadVCA(const String &VCAPath);

protected:
	static void _bind_methods();

public:
	/* system functions */
	void init(int numOfChannels, int studioFlags, int flags);
	void update();
	void shutdown();
	void addListener(Object *gameObj);
	void setSoftwareFormat(int sampleRate, int speakerMode, int numRawSpeakers);
	void setGlobalParameter(const String &parameterName, float value);
	float getGlobalParameter(const String &parameterName);

	/* helper functions for playing sounds in 3D */
	void playOneShot(const String &eventName, Object *gameObj);
	void playOneShotWithParams(const String &eventName, Object *gameObj, const Dictionary &parameters);
	void playOneShotAttached(const String &eventName, Object *gameObj);
	void playOneShotAttachedWithParams(const String &eventName, Object *gameObj, const Dictionary &parameters);
	void attachInstanceToNode(const String &uuid, Object *gameObj);
	void detachInstanceFromNode(const String &uuid);

	/* bank functions */
	String loadbank(const String &pathToBank, int flags);
	void unloadBank(const String &pathToBank);
	int getBankLoadingState(const String &pathToBank);
	int getBankBusCount(const String &pathToBank);
	int getBankEventCount(const String &pathToBank);
	int getBankStringCount(const String &pathToBank);
	int getBankVCACount(const String &pathToBank);

	/* event functions */
	String createEventInstance(const String &uuid, const String &eventPath);
	float getEventParameter(const String &uuid, const String &parameterName);
	void setEventParameter(const String &uuid, const String &parameterName, float value);
	void releaseEvent(const String &uuid);
	void startEvent(const String &uuid);
	void stopEvent(const String &uuid, int stopMode);
	void triggerEventCue(const String &uuid);
	int getEventPlaybackState(const String &uuid);
	bool getEventPaused(const String &uuid);
	void setEventPaused(const String &uuid, bool paused);
	float getEventPitch(const String &uuid);
	void setEventPitch(const String &uuid, float pitch);
	float getEventVolume(const String &uuid);
	void setEventVolume(const String &uuid, float volume);
	int getEventTimelinePosition(const String &uuid);
	void setEventTimelinePosition(const String &uuid, int position);
	float getEventReverbLevel(const String &uuid, int index);
	void setEventReverbLevel(const String &uuid, int index, float level);
	bool isEventVirtual(const String &uuid);

	/* bus functions */
	bool getBusMute(const String &busPath);
	bool getBusPaused(const String &busPath);
	float getBusVolume(const String &busPath);
	void setBusMute(const String &busPath, bool mute);
	void setBusPaused(const String &busPath, bool paused);
	void setBusVolume(const String &busPath, float volume);
	void stopAllBusEvents(const String &busPath, int stopMode);

	/* VCA functions */
	float getVCAVolume(const String &VCAPath);
	void setVCAVolume(const String &VCAPath, float volume);

	/* Sound functions */
	void playSound(const String &uuid);
	String loadSound(const String &uuid, const String &path, int mode);
	void releaseSound(const String &path);
	void setSoundPaused(const String &uuid, bool paused);
	void stopSound(const String &uuid);
	bool isSoundPlaying(const String &uuid);
	void setSoundVolume(const String &uuid, float volume);
	float getSoundVolume(const String &uuid);
	float getSoundPitch(const String &uuid);
	void setSoundPitch(const String &uuid, float pitch);

	void setSound3DSettings(float dopplerScale, float distanceFactor, float rollOffScale);

	Fmod();
	~Fmod();
};

#endif
