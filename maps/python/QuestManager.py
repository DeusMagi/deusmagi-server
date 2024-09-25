"""
Quest manager related implementations.
"""

import time

import Atrinik


QUEST_TYPE_ATTRIBUTES = frozenset([
    ("item", Atrinik.QUEST_TYPE_ITEM),
    ("kill", Atrinik.QUEST_TYPE_KILL),
    ("special", Atrinik.QUEST_TYPE_SPECIAL),
])


class QuestManager:
    """
    Simple quest manager implementation.
    """
    
    quest = None;
    quest_object = None;
    
    def __init__(self, activator, quest):
        """
        Initialize the quest manager.

        :param activator: Player object.
        :type activator: :class:`Atrinik.Object.Object`
        :param quest: Quest information dictionary.
        :type quest: dict
        """

        if activator is None or activator.type != Atrinik.Type.PLAYER:
            return

        self.activator = activator
        self.quest = quest
        self.sound_last = None

        self.quest_container = activator.Controller().quest_container
        self.quest_object = self.quest_container.FindObject(
            name=self.quest.get("uid")
        )
        self.reset_quest()

    @staticmethod
    def create_quest_object(where, quest, uid):
        """
        Stores quest part information as an object in the specified object's
        inventory.

        :param where: Where to store the quest part information object.
        :type where: :class:`Atrinik.Object.Object`
        :param quest: Quest part information.
        :type quest: dict
        :param uid: Unique identifier of the quest part.
        :param uid: str
        :return: The created object.
        :rtype: :class:`Atrinik.Object.Object`
        """

        obj = where.CreateObject("quest_container")
        obj.magic = 0
        obj.last_grace = 0
        obj.name = uid
        obj.race = quest.get("name")
        obj.msg = quest.get("info")

        for attr, quest_type in QUEST_TYPE_ATTRIBUTES:
            if attr in ("item", "kill"):
                val = quest.get(attr)
                if val:
                    obj.sub_type = quest_type
                    obj.last_grace = val.get("nrof", 1)
                    break
            elif attr == "special":
                obj.sub_type = quest_type

        return obj

    def sound(self, sound):
        """
        Play the specified sound file. If called multiple times with the same
        sound file, the extraneous calls are suppressed.

        :param sound: Sound file to play, eg, 'learnspell.ogg'.
        :type sound: str
        """

        if self.sound_last == sound:
            return

        self.activator.Controller().Sound(sound)
        self.sound_last = sound

    def reset_quest(self):
        """
        Attempts to reset quests that allow repeat.
        """

        # Not a repeat quest, nothing to do.
        if not self.quest.get("repeat", False):
            return

        # No remaining quest points, nothing to do yet.
        if self.get_qp_remaining() == 0:
            return

        # The quest must be completed.
        if not self.completed():
            return

        # Enough time must pass before the quest can be repeated.
        if self.quest_object.exp != 0:
            if int(time.time()) < self.quest_object.exp:
                return

        self.quest_object.Destroy()
        self.quest_object = None

    def get_qp_max(self):
        """
        Acquires the maximum available quest points for the activator.

        :return: Maximum available quest points.
        :rtype: int
        """

        return int(max(1, self.activator.level * 0.35 + 0.5))

    def get_qp_restored(self):
        """
        Calculate how many quest points should be restored.

        :return: Number of quest points to restore.
        :rtype: int
        """

        qp_max = self.get_qp_max()
        secs = time.time() - self.quest_container.exp
        return min(int(secs / (60.0 * 60.0) * (qp_max * 0.10)), qp_max)

    def get_qp_current(self):
        """
        Acquire how many quest points have been used up.

        :return: Number of used up quest points.
        :rtype: int
        """

        return self.quest_container.magic

    def get_qp_remaining(self):
        """
        Acquire remaining quest points.

        :return: Remaining quest points.
        :rtype: int
        """

        # Try to restore quest points.
        restored = self.get_qp_restored()
        if restored != 0:
            qp_current = max(0, self.quest_container.magic - restored)
            self.quest_container.magic = qp_current
            self.quest_container.exp = int(time.time())

        return max(0, self.get_qp_max() - self.get_qp_current())

    def use_qp(self):
        """
        Attempt to use up quest points due to completing the quest.
        """

        # Not a repeat quest.
        if not self.quest.get("repeat", False):
            return

        self.quest_container.magic += 1

        if self.quest_container.exp == 0:
            self.quest_container.exp = int(time.time())

        delay = self.quest.get("repeat_delay", None)
        assert(isinstance(delay, (int, None)))

        if delay:
            self.quest_object.exp = int(time.time()) + delay

    def remove_quest_items(self, quest, obj):
        """
        Removes quest items associated with the quest part from the activator's
        inventory.

        :param quest: The actual quest part info.
        :type quest: dict
        :param obj: The quest part object.
        :type obj: :class:`Atrinik.Object.Object`
        """

        # Only do this for kill item quest types.
        if obj.sub_type != Atrinik.QUEST_TYPE_ITEM:
            return

        # Are we going to keep the quest item(s)?
        keep = quest["item"].get("keep")
        nrof = quest["item"].get("nrof", 1)
        removed = 0

        # Find all matching objects.
        for tmp in self.activator.FindObjects(
                Atrinik.INVENTORY_CONTAINERS, quest["item"]["arch"],
                quest["item"]["name"]):
            # Keeping the quest item(s), make sure quest-related flags
            # are not set.
            if keep:
                tmp.f_quest_item = False
                tmp.f_startequip = False

                if tmp.f_soulbound:
                    tmp.WriteKey("soulbound_name", self.activator.name)
            else:
                remove = min(max(1, tmp.nrof), nrof - removed)
                tmp.Decrease(remove)
                removed += remove

                if removed == nrof:
                    break

    def get_quest_item_num(self, quest):
        """
        Acquire the number of quest items the activator has found.

        :param quest: The quest part info.
        :type quest: dict
        :return: Number of quest items.
        :rtype: int
        """

        # Just one item, easy.
        if quest["item"].get("nrof", 1) <= 1:
            quest_item = self.activator.FindObject(
                Atrinik.INVENTORY_CONTAINERS, quest["item"]["arch"],
                quest["item"]["name"]
            )

            if quest_item:
                return 1
        # Got to count the objects otherwise.
        else:
            num = 0

            # Find all matching objects, and count them.
            for tmp in self.activator.FindObjects(
                    Atrinik.INVENTORY_CONTAINERS, quest["item"]["arch"],
                    quest["item"]["name"]):
                num += max(1, tmp.nrof)

            return num

        return 0

    def num2finish(self, part):
        """
        Acquire the necessary number of items/kills/etc in order to complete
        the quest.

        :param part: Quest part specifier.
        :type part: str or list
        :return: Number of kills/items/etc still required to complete the quest.
                 If zero, the quest (part) can be completed.
        :rtype: int
        """

        part, quest = self.get_part(part)

        if "kill" in quest:
            nrof = quest["kill"].get("nrof", 1)

            if not self.quest_object:
                return nrof

            obj = self.quest_object.FindObject(name=part)
            if not obj:
                return nrof

            return max(0, nrof - obj.last_sp)
        elif "item" in quest:
            nrof = quest["item"].get("nrof", 1)
            return max(0, nrof - self.get_quest_item_num(quest))

        return 0

    def get_quest_status(self, part=None):
        """
        Acquire quest status of the specified quest (part).

        :param part: Quest part specifier. If None, will get the overall quest
                     status itself.
        :type part: str or list or None
        :return: The quest status.
        :rtype: int
        """

        if part is None:
            return self.quest_object.magic

        part, quest = self.get_part(part)
        obj = self.quest_object.FindObject(name=part)
        if obj is None:
            return Atrinik.QUEST_STATUS_INVALID

        return obj.magic

    def any_started(self):
        """
        Checks if the quest has any started parts. Child parts are not checked.

        :return: True if there are any started quest parts, False otherwise.
        :rtype: bool
        """

        assert self.quest_object

        for obj in self.quest_object.inv:
            if obj.magic == Atrinik.QUEST_STATUS_STARTED:
                return True

        return False

    def need_start(self, part):
        """
        Checks whether the specified quest part needs to be started.

        :param part: Quest part specifier.
        :type part: str or list
        :return: Whether the quest has yet to be started.
        :rtype: bool
        """

        if self.started(part):
            return False

        return True

    def need_finish(self, part):
        """
        Checks whether the specified quest part needs to be finished (not yet
        complete, and the criteria to complete it are not yet met).

        :param part: Quest part specifier.
        :type part: str or list
        :return: Whether the quest needs to be finished first.
        :rtype: bool
        """

        if not self.started(part):
            return False

        if self.finished(part):
            return False

        if self.completed(part):
            return False

        return True

    def need_complete(self, part):
        """
        Checks whether the specified quest part needs to completed (finishing
        criteria have been met).

        :param part: Quest part specifier.
        :type part: str or list
        :return: Whether the quest needs to be completed.
        :rtype: bool
        """

        if not self.started(part):
            return False

        if not self.finished(part):
            return False

        if self.completed(part):
            return False

        if self.failed(part):
            return False

        return True

    def need_complete_before_start(self, part):
        """
        Checks whether the specified quest part needs to completed (finishing
        criteria have been met) prior to even starting the quest.

        :param part: Quest part specifier.
        :type part: str or list
        :return: Whether the quest needs to be completed before it's even begun.
        :rtype: bool
        """

        if self.started(part):
            return False

        if self.completed(part):
            return False

        if self.failed(part):
            return False

        if not self.finished(part):
            return False

        return True

    def get_part(self, part):
        """
        Acquire quest part definition entry from a quest part specifier.

        Since the quest parts are stored in a multi-level dictionary, it is
        often desirable to access a specific quest part and its info, for
        example::

            > qm.qet_part(['part1', 'part2'])
            ('part2', {'info': 'this is the quest part info'})

        :param part: Quest part specifier.
        :type part: str or list
        :return: Tuple containing the bottommost quest part UID and the quest
                 part dictionary.
        :rtype: tuple
        """

        if type(part) is str:
            return part, self.quest["parts"][part]

        new_part = self.quest

        for val in part:
            new_part = new_part["parts"][val]

        return part[-1], new_part

    def ensure_quest_object(self):
        """
        Ensure the main quest object exists, if not, create it.
        """

        if self.quest_object:
            return

        self.quest_object = self.create_quest_object(
            self.quest_container, self.quest, self.quest.get("uid")
        )

    def start(self, part, sound="learnspell.ogg"):
        """
        Begins the specified quest part.

        :param part: Quest part specifier.
        :type part: str or list
        :param sound: Sound file to play.
        :type sound: str
        """

        self.ensure_quest_object()
        part, quest = self.get_part(part)
        self.create_quest_object(self.quest_object, quest, part)
        self.sound(sound)

    def complete(self, part, sound="learnspell.ogg"):
        """
        Completes the specified quest part.

        :param part: Quest part specifier.
        :type part: str or list
        :param sound: Sound file to play.
        :type sound: str
        :return: True if the entire quest was completed, False otherwise.
        :rtype: bool
        """

        assert self.quest_object

        part, quest = self.get_part(part)
        obj = self.quest_object.FindObject(name=part)
        if not obj:
            return False

        # Mark the quest part as completed.
        obj.magic = Atrinik.QUEST_STATUS_COMPLETED
        self.sound(sound)
        self.remove_quest_items(quest, obj)

        # Check all quest parts. If all are completed, complete the
        # entire quest.
        if self.any_started():
            return False

        # Complete the quest itself now.
        self.quest_object.magic = Atrinik.QUEST_STATUS_COMPLETED
        self.use_qp()
        return True

    def fail(self, part, sound="warning_statdown.ogg"):
        """
        Fails the specified quest part.

        :param part: Quest part specifier.
        :type part: str or list
        :param sound: Sound file to play.
        :type sound: str
        :return: True if the entire quest was failed, False otherwise.
        :rtype: bool
        """

        assert self.quest_object

        part, quest = self.get_part(part)
        obj = self.quest_object.FindObject(name=part)
        if not obj:
            return False

        # Mark the quest part as failed.
        obj.magic = Atrinik.QUEST_STATUS_FAILED
        self.sound(sound)
        self.remove_quest_items(quest, obj)

        # Check all quest parts. If any are still started, no reason to fail
        # the entire quest just yet.
        if self.any_started():
            return False

        # Fail the quest itself now.
        self.quest_object.magic = Atrinik.QUEST_STATUS_COMPLETED
        self.use_qp()
        return True

    def finished(self, part):
        """
        Checks if the specified quest part is finished (criteria are met in
        order to turn it in).

        :param part: Quest part specifier.
        :type part: str or list
        :return: Whether the quest part is finished.
        :rtype: bool
        """

        return self.num2finish(part) == 0

    def started(self, part=None):
        """
        Checks if the specified quest part has been started.

        :param part: Quest part specifier. If None, will check whether the quest
                     itself has been started at all.
        :type part: str or list or None
        :return: Whether the quest (part) has been started.
        :rtype: bool
        """

        if not self.quest_object:
            return False

        if part is not None:
            part, quest = self.get_part(part)
            return self.quest_object.FindObject(name=part) is not None

        return True

    def completed(self, part=None):
        """
        Checks if the specified quest part has been completed.

        :param part: Quest part specifier. If None, will check whether the quest
                     itself has been fully completed.
        :type part: str or list or None
        :return: Whether the quest (part) has been completed.
        :rtype: bool
        """

        if not self.started(part):
            return False

        return self.get_quest_status(part) == Atrinik.QUEST_STATUS_COMPLETED

    def failed(self, part=None):
        """
        Checks if the specified quest part has been completed.

        :param part: Quest part specifier. If None, will check whether the quest
                     itself has been fully completed.
        :type part: str or list or None
        :return: Whether the quest (part) has been completed.
        :rtype: bool
        """

        if not self.started(part):
            return False

        return self.get_quest_status(part) == Atrinik.QUEST_STATUS_FAILED

    def state_set(self, key, value):
        """
        Store a key/value based quest state, regardless of quest parts.

        :param key: Unique identifier.
        :type key: str
        :param value: Value to store.
        :type value: str or int or float or bool
        """

        assert isinstance(value, (str, int, float, bool))

        if isinstance(value, bool):
            value = int(value)

        value = str(value)
        self.ensure_quest_object()
        self.quest_object.WriteKey(key, value)

    def state_get(self, key):
        """
        Acquire previously stored quest state.

        :param key: Unique identifier.
        :type key: str
        :return: Previously stored value.
        :rtype: str
        """

        if self.quest_object is None:
            return None

        return self.quest_object.ReadKey(key)

    def state_get_int(self, key):
        """
        Acquire previously stored quest state as an integer.

        :param key: Unique identifier.
        :type key: str
        :return: Previously stored value.
        :rtype: int
        """

        value = self.state_get(key)
        if value is None:
            return 0

        try:
            return int(value)
        except ValueError:
            return 0

    def state_get_float(self, key):
        """
        Acquire previously stored quest state as a float.

        :param key: Unique identifier.
        :type key: str
        :return: Previously stored value.
        :rtype: float
        """

        value = self.state_get(key)
        if value is None:
            return 0.0

        try:
            return float(value)
        except ValueError:
            return 0.0

    def state_get_bool(self, key):
        """
        Acquire previously stored quest state as a boolean.

        :param key: Unique identifier.
        :type key: str
        :return: Previously stored value.
        :rtype: bool
        """

        return bool(self.state_get_float(key))
