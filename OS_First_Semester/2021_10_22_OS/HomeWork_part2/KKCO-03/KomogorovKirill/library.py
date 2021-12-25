#!/usr/bin/python3
# -*- coding: utf-8 -*-

from fastapi import FastAPI, HTTPException
import uvicorn
import json
import asyncio
from pydantic import BaseModel

lib = {
"Лермонтов": {
	"Осень":
"Листья в поле пожелтели,\n\
И кружатся и летят;\n\
Лишь в бору поникши ели\n\
Зелень мрачную хранят.\n\
Под нависшею скалою\n\
Уж не любит, меж цветов,\n\
Пахарь отдыхать порою\n\
От полуденных трудов.\n\
Зверь, отважный, поневоле\n\
Скрыться где-нибудь спешит.\n\
Ночью месяц тускл, и поле\n\
Сквозь туман лишь серебрит.\n",

	"Утёс":
"Ночевала тучка золотая\n\
На груди утеса-великана;\n\
Утром в путь она умчалась рано,\n\
По лазури весело играя;\n\
Но остался влажный след в морщине\n\
Старого утеса. Одиноко\n\
Он стоит, задумался глубоко,\n\
И тихонько плачет он в пустыне.\n",

"Из Гете":
"Горные вершины\n\
Спят во тьме ночной;\n\
Тихие долины\n\
Полны свежей мглой;\n\
Не пылит дорога,\n\
Не дрожат листы…\n\
Подожди немного,\n\
Отдохнёшь и ты.\n"},
"Блок": {
	"Кошмар":
"Я проснулся внезапно в ночной тишине,\n\
И душа испугалась молчания ночи.\n\
Я увидел на темной стене\n\
Чьи-то скорбные очи.\n\
Без конца на пустой и безмолвной стене\n\
Эти полные скорби и ужаса очи\n\
Всё мерещатся мне в тишине\n\
Леденеющей ночи.\n",

"Ночь, улица, фонарь, аптека":
"Ночь, улица, фонарь, аптека,\n\
Бессмысленный и тусклый свет.\n\
Живи еще хоть четверть века —\n\
Все будет так. Исхода нет.\n\
Умрешь — начнешь опять сначала\n\
И повторится все, как встарь:\n\
Ночь, ледяная рябь канала,\n\
Аптека, улица, фонарь.\n",

"Зачем, зачем во мрак небытия":
"Зачем, зачем во мрак небытия\n\
Меня влекут судьбы удары?\n\
Ужели всё, и даже жизнь моя —\n\
Одни мгновенья долгой кары?\n\
Я жить хочу, хоть здесь и счастья нет,\n\
И нечем сердцу веселиться,\n\
Но всё вперед влечет какой-то свет,\n\
И будто им могу светиться!\n\
Пусть призрак он, желанный свет вдали!\n\
Пускай надежды все напрасны!\n\
Но там,- далёко суетной земли,-\n\
Его лучи горят прекрасно!\n"},
"Маяковский": {
	"Горе":
"Тщетно отчаянный ветер\n\
бился нечеловече.\n\
Капли чернеющей крови\n\
стынут крышами кровель.\n\
И овдовевшая в ночи\n\
вышла луна одиночить.\n",

    "Уже второй":
"Уже второй. Должно быть, ты легла.\n\
В ночи Млечпуть серебряной Окою.\n\
Я не спешу, и молниями телеграмм\n\
Мне незачем тебя будить и беспокоить.\n\
Как говорят, инцидент исперчен.\n\
Любовная лодка разбилась о быт.\n\
С тобой мы в расчете. И не к чему перечень\n\
Взаимных болей, бед и обид.\n\
Ты посмотри, какая в мире тишь.\n\
Ночь обложила небо звездной данью.\n\
В такие вот часы встаешь и говоришь\n\
Векам, истории и мирозданью.\n",

    "Я":
"По мостовой\n\
моей души изъезженной\n\
шаги помешанных\n\
вьют жестких фраз пяты.\n\
Где города\n\
повешены\n\
и в петле облака\n\
застыли\n\
башен\n\
кривые выи —\n\
иду\n\
один рыдать,\n\
что перекрестком\n\
распяты\n\
городовые.\n"}
}

app = FastAPI()

@app.get("/library")
async def library():
    return {'author': list(lib.keys())}

@app.get("/library/{author}")
async def libauthor(author):
    try:
        return {'author': list(lib[author].keys())}
    except KeyError:
        return HTTPException(404)

@app.get("/library/{author}/{work}")
async def worksplit(author, work, begin: str = "", end: str = ""):
    if author not in list(lib.keys()):
        return HTTPException(404)
    if work not in list(lib[author].keys()):
        return HTTPException(404)
    if isinstance(lib[author][work], bytearray):
        words = str((lib[author][work]).decode('utf-8'))
    else:
        words = str(lib[author][work])    
    if begin == "" or end == "":
        return {'poem': words}
    begin = words.find(begin)
    end = words.rfind(end) + 1
    if begin != -1 and end != 0 and begin <= end:
        return {'poem': words[begin:end]+"\n"}
    else:
        return {'poem': words}

@app.post("/")
async def new_author(author: str):
    if author not in list(lib.keys()):
        lib[author] = {}

@app.post("/{author}")
async def new_poem(author, work: str = ""):
    try:
        test = lib[author]
    except KeyError:
        return HTTPException(404)
    if work not in list(lib[author].keys()):
        lib[author][work] = bytearray()

@app.put("/{author}/{work}")
async def correct_poem(author, work, poem: int = 0, count: int = 0):
    try:
        if poem != "" and poem != 0:
            if count == 0:
                lib[author][work] = bytearray()
            lib[author][work].append(poem)
    except KeyError:
        return HTTPException(404)

if __name__ == "__main__":
    uvicorn.run(app)
