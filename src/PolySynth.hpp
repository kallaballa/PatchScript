#pragma once

#include "Tonic.h"
#include <mutex>

using namespace Tonic;

template<typename VoiceAllocator>
class PolySynthWithAllocator: public Synth {
private:
	std::mutex polyMutex_;
public:
	PolySynthWithAllocator() :
			Synth() {
		setOutputGen(mixer);
	}

	virtual ~PolySynthWithAllocator() {

	}

	void addVoice(Synth synth) {
		std::scoped_lock lock(polyMutex_);
		allocator.addVoice(synth);
		mixer.addInput(synth);
	}

	std::vector<typename VoiceAllocator::PolyVoice>& getVoices() {
		std::scoped_lock lock(polyMutex_);
		return allocator.getVoices();
	}

	typedef Synth (VoiceCreateFn)();
	void addVoices(VoiceCreateFn createFn, int count) {
		std::scoped_lock lock(polyMutex_);
		for (int i = 0; i < count; i++)
			addVoice(createFn());
	}

	void noteOn(int note, int velocity) {
		std::scoped_lock lock(polyMutex_);
		allocator.noteOn(note, velocity);
	}

	void noteOff(int note) {
		std::scoped_lock lock(polyMutex_);
		allocator.noteOff(note);
	}

	void clearAllNotes() {
		std::scoped_lock lock(polyMutex_);
		allocator.clearAllNotes();
	}
protected:
	Mixer mixer;
	VoiceAllocator allocator;
};

class BasicPolyphonicAllocator {
public:
	class PolyVoice {
	public:
		int currentNote;
		Synth synth;
	};

	virtual ~BasicPolyphonicAllocator() {

	}
	void addVoice(Synth synth);
	void noteOn(int noteNumber, int velocity);
	void noteOff(int noteNumber);
	void clearAllNotes();

	vector<PolyVoice>& getVoices();
protected:
	virtual int getNextVoice(int note);
	vector<PolyVoice> voiceData;
	list<int> inactiveVoiceQueue;
	list<int> activeVoiceQueue;
};

class OldestNoteStealingPolyphonicAllocator: public BasicPolyphonicAllocator {
public:
	virtual ~OldestNoteStealingPolyphonicAllocator() {

	}
protected:
	virtual int getNextVoice(int note);
};

class LowestNoteStealingPolyphonicAllocator: public BasicPolyphonicAllocator {
public:
	virtual ~LowestNoteStealingPolyphonicAllocator() {

	}
protected:
	virtual int getNextVoice(int note);
};

typedef PolySynthWithAllocator<LowestNoteStealingPolyphonicAllocator> PolySynth;

