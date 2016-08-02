
from models import Topic

def addTopic(inTitle):
    newTopic = Topic()
    newTopic.title = inTitle
    newTopic.save()
    return newTopic

        