
from models import Topic

def addTopic(inTitle):
    try:
        existingTopic = Topic.objects.get(title=inTitle)
        return False
    except Topic.DoesNotExist: #this is a good thing! We can create an topic now!
        newTopic = Topic()
        newTopic.title = inTitle
        newTopic.save()
        return newTopic
    
    

        