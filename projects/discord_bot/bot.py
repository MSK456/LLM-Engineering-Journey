import discord
from groq import Groq
import os
import urllib.parse
from dotenv import load_dotenv
from gtts import gTTS  # 🎤 NEW: Google Text-to-Speech

load_dotenv()
DISCORD_TOKEN = os.getenv("DISCORD_TOKEN")
GROQ_API_KEY = os.getenv("GROQ_API_KEY")

client = Groq(api_key=GROQ_API_KEY)

intents = discord.Intents.default()
intents.message_content = True
bot = discord.Client(intents=intents)

chat_history = []

@bot.event
async def on_ready():
    print(f"🚀 {bot.user} is online! Ready to chat, paint, AND speak!")

@bot.event
async def on_message(message):
    global chat_history 
    
    if message.author == bot.user:
        return

    if bot.user.mentioned_in(message):
        user_message = message.content.replace(f'<@{bot.user.id}>', '').strip()
        
        # ---------------------------------------------------------
        # 🎨 THE PAINTBRUSH
        # ---------------------------------------------------------
        if user_message.lower().startswith("draw") or user_message.lower().startswith("paint"):
            async with message.channel.typing():
                prompt = user_message.lower().replace("draw", "", 1).replace("paint", "", 1).strip()
                safe_prompt = urllib.parse.quote(prompt)
                image_url = f"https://image.pollinations.ai/prompt/{safe_prompt}"
                await message.channel.send(f"🎨 **Here is your masterpiece:** '{prompt}'\n{image_url}")
            return

        # ---------------------------------------------------------
        # 🎤 THE VOCAL CORDS (NEW)
        # ---------------------------------------------------------
        is_voice_request = False
        if user_message.lower().startswith("speak") or user_message.lower().startswith("say"):
            is_voice_request = True
            # Remove the trigger word so Groq doesn't get confused
            user_message = user_message.lower().replace("speak", "", 1).replace("say", "", 1).strip()

        # ---------------------------------------------------------
        # 🧠 THE BRAIN (Groq Chat + Memory)
        # ---------------------------------------------------------
        chat_history.append({"role": "user", "content": user_message})
        
        if len(chat_history) > 10:
            chat_history.pop(0)

        async with message.channel.typing():
            try:
                system_prompt = {
                    "role": "system", 
                    "content": "You are a sarcastic, witty AI friend in a Discord server. Give short, punchy responses. ALWAYS include multiple fun emojis."
                }
                
                messages_payload = [system_prompt] + chat_history
                
                response = client.chat.completions.create(
                    model="llama-3.1-8b-instant", 
                    messages=messages_payload,
                    temperature=0.8 
                )
                
                reply = response.choices[0].message.content
                chat_history.append({"role": "assistant", "content": reply})
                
                # If the user asked it to speak, convert the reply to an MP3!
                if is_voice_request:
                    # Create the audio file
                    tts = gTTS(text=reply, lang='en', slow=False)
                    # We just overwrite the same file every time to save hard drive space
                    audio_filename = "jester_voice.mp3"
                    tts.save(audio_filename)
                    
                    # Send the text AND the audio file together
                    await message.channel.send(content=f"🗣️ **Voice Message:**\n{reply}", file=discord.File(audio_filename))
                else:
                    # Otherwise, just send normal text
                    await message.channel.send(reply)
                
            except Exception as e:
                print(f"🚨 GROQ ERROR: {e}")
                await message.channel.send("Bro, my AI brain just short-circuited. Check the VS Code terminal! 🤖💨")

bot.run(DISCORD_TOKEN)